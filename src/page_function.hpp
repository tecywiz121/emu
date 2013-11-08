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

#include <cstdint>
#include "page.hpp"

namespace tomato
{

class page_function : public jit_function
{
private:
    uint8_t _stop_sentinel;
    jit_value _R[7];
    const page& _my_page;

    uint32_t _real_registers[7];

protected:
    virtual jit_type_t create_signature() override;

    virtual void emit_instruction(uint16_t insn);
    virtual void emit_save_state();

public:
    page_function(jit_context& context, const page& my_page)
        : jit_function(context), _my_page(my_page)
    {
        create();
    }

    const page& my_page() const { return _my_page; }

    virtual void build() override;
};

}

#endif
