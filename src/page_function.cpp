/* Copyright 2013 Sam Wilson
 *
 * This file is part of Tomato.
 *
 * Tomato is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tomato is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tomato.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include "page_function.hpp"
#include "jit/jit.h"
#include "jit/jit-plus.h"

namespace tomato
{

jit_type_t page_function::create_signature()
{
    return signature_helper(
        jit_type_uint,  // Return       the index of the last executed instruction
        jit_type_uint,  // Argument     the index of the instruction to resume from
        end_params
    );
}

void page_function::build()
{
    // Build array of labels to use for each instruction
    jit_label_t bytecode_labels[_page.size()];
    for (jit_label_t& lbl : bytecode_labels) {
        lbl = jit_label_undefined;
    }

    jit_value resume = get_param(0);
    jit_value ret_err = new_constant(0xFFFFFFFF); // TODO: put this into a constant somewhere
    jit_value ret_done = new_constant(0xFFFFFFFE);
    jit_value stop_sentinel_ptr = new_constant(&_stop_sentinel);

    // Page preamble
    // TODO: Check return value
    jit_insn_jump_table(raw(), resume.raw(), bytecode_labels, _page.size());    // Jump to the instruction
    insn_return(ret_err);                                                       // Invalid jump destination

    // Instructions
    const std::vector<uint16_t>& memory = _page.memory();
    for (std::vector<uint16_t>::size_type ii = 0; ii < memory.size(); ii++) {
        // Instruction
        jit_insn_label(raw(), &bytecode_labels[ii]);                            // Create the label for this instruction
        emit_instruction(memory.at(ii));                                        // Emit the native code

        // Epilog
        if (ii+1 == memory.size()) {                                            // Check if this is the last instruction in the page
            emit_save_state();                                                  // If it is, save the current state of the CPU
            insn_return(ret_done);                                              // and return value signifying the end of the page.
        } else {                                                                // Otherwise, check if we need to pause execution
            // Not the last instruction

            jit_value stop = insn_load_relative(stop_sentinel_ptr, 0,
                                                jit_type_ubyte);                // Load the value of the stop sentinel
            // TODO: Check return value
            jit_insn_branch_if_not(raw(), stop.raw(), &bytecode_labels[ii+1]);  // Check if the stop sentinel has been set
            emit_save_state();                                                  // If it has, save the current state of the CPU
            jit_value next = new_constant(ii+1, jit_type_uint);                 // and return the offset of the next instruction
            insn_return(next);                                                  // Return it
                                                                                // Otherwise, go to the next instruction
        }
    }

}

void page_function::emit_save_state()
{
    for (int ii = 0; ii < 7; ii++) {
        jit_value addr = new_constant(&_real_registers[ii]);
        insn_store_relative(addr, 0, _R[ii]);
    }
}

void page_function::emit_instruction(uint16_t insn)
{
    switch (insn) {
    case 0x0000:
        break;

    case 0x0001:
        _R[0] = _R[0] + new_constant(1);
        break;
    }
}

}
