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
#include <memory>
#include "page.hpp"
#include "util.hpp"

namespace tomato
{

void page::permissions(page_flags flags)
{
    bool ex = executable();             // Save the old value of executable

    _flags &= ~page_flags::EXECUTABLE;  // Clear the lowest two bits
    _flags |= (flags & 0x03);           // Set the permission bits

    if (ex) {
        if (!executable()) {
            _function.reset(0);
        }
    } else {
        if (executable()) {
            _function = make_unique<page_function>(*this);
        }
    }
}
}
