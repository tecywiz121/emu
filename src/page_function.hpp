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
#ifndef PAGE_FUNCTION_HPP
#define PAGE_FUNCTION_HPP 1

#include "jit/jit-plus.h"
#include "computer.hpp"

#include <cstdint>

namespace tomato
{

class page;

class page_function : public jit_function
{
private:
    typedef jit_value registers[computer::N_REGISTERS];
    const page& _my_page;

protected:
    virtual jit_type_t create_signature() override;

    virtual void emit_instruction(uint16_t insn, registers& R);
    virtual void emit_save_state(registers& R);

public:
    enum return_code {
        DONE = 0xFFFFFFFE,
        ERROR = 0xFFFFFFFF,
    };

    page_function(page& my_page);

    const page& my_page() const { return _my_page; }

    virtual void build() override;
};

}

#endif
