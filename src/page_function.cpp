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
#include <vector>
#include "page_function.hpp"
#include "page.hpp"
#include "jit/jit.h"
#include "jit/jit-plus.h"

using std::vector;

namespace tomato
{

page_function::page_function(page& my_page)
    : jit_function(my_page.parent().jit_ctx()), _my_page(my_page)
{
    create();
}

jit_type_t page_function::create_signature()
{
    return signature_helper(
        jit_type_uint,      // Return       the index of the last executed instruction
        jit_type_uint,      // Argument     the index of the instruction to resume from
        jit_type_void_ptr,  // Argument     stop_sentinel
        jit_type_void_ptr,  // Argument     pointer to register A
        jit_type_void_ptr,  // Argument     pointer to register B
        jit_type_void_ptr,  // Argument     pointer to register C
        jit_type_void_ptr,  // Argument     pointer to register D
        jit_type_void_ptr,  // Argument     pointer to register E
        jit_type_void_ptr,  // Argument     pointer to register F
        jit_type_void_ptr,  // Argument     pointer to register PC
        end_params
    );
}

void page_function::build()
{
    // Read Parameters
    jit_value resume = get_param(0);
    jit_value stop_sentinel_ptr = get_param(1);

    // Initialize the registers
    registers R;
    for (int ii = 0; ii < computer::N_REGISTERS; ii++) {
        R[ii] = insn_load_relative(get_param(ii+2), 0, jit_type_uint);
    }

    // Build array of labels to use for each instruction
    vector<jit_label_t> bytecode_labels;
    for (vector<jit_label_t>::size_type ii = 0; ii < _my_page.size(); ii++) {
        bytecode_labels.push_back(jit_label_undefined);
    }

    jit_value ret_err = new_constant(return_code::ERROR);
    jit_value ret_done = new_constant(return_code::DONE);

    // Page preamble
    // TODO: Check return value
    jit_insn_jump_table(raw(), resume.raw(), &bytecode_labels[0],
                        static_cast<unsigned int>(_my_page.size()));            // Jump to the instruction
    insn_return(ret_err);                                                       // Invalid jump destination

    // Instructions
    const std::vector<uint16_t>& memory = _my_page.memory();
    for (std::vector<uint16_t>::size_type ii = 0; ii < memory.size(); ii++) {
        // Instruction
        jit_insn_label(raw(), &bytecode_labels[ii]);                            // Create the label for this instruction
        emit_instruction(memory.at(ii), R);                                     // Emit the native code

        // Instruction epilog
        if (ii+1 < memory.size()) {                                             // Check if this is the last instruction in the page
                                                                                // Not the last instruction, so check if we need to pause

            jit_value stop = insn_load_relative(stop_sentinel_ptr, 0,
                                                jit_type_sys_bool);             // Load the value of the stop sentinel
            // TODO: Check return value
            jit_insn_branch_if_not(raw(), stop.raw(), &bytecode_labels[ii+1]);  // Check if the stop sentinel has been set
            emit_save_state(R);                                                 // If it has, save the current state of the CPU
            jit_value next = new_constant(ii+1, jit_type_uint);                 // and return the offset of the next instruction
            insn_return(next);                                                  // Return it
                                                                                // Otherwise, go to the next instruction
        }
    }

    // Page epilog
    emit_save_state(R);                                                         // Save the current state of the CPU
    insn_return(ret_done);                                                      // and return value signifying the end of the page.

}

void page_function::emit_save_state(registers& R)
{
    for (int ii = 0; ii < computer::N_REGISTERS; ii++) {
        insn_store_relative(get_param(ii+2), 0, R[ii]);
    }
}

void page_function::emit_instruction(uint16_t insn, registers& R)
{
    switch (insn) {
    case 0x0000:
        break;

    case 0x0001:
        R[0] = new_constant(0x00);
        R[1] = new_constant(0x01);
        R[2] = new_constant(0x02);
        R[3] = new_constant(0x03);
        R[4] = new_constant(0x04);
        R[5] = new_constant(0x05);
        R[6] = new_constant(0x06);
        break;
    }
}

}
