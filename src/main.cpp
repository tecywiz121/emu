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
#include <iostream>
#include "jit/jit-plus.h"

#include "computer.hpp"
#include "page.hpp"
#include "page_function.hpp"

using std::cout;
using std::endl;

using namespace tomato;

struct state
{
    jit_uint A;
    jit_uint B;
    jit_uint C;
    jit_uint D;
    jit_uint E;
    jit_uint F;
    jit_uint PC;
};

int main(int, char**)
{
    computer system;
    page test(system, 1);
    test.memory()[0] = 1;
    test.permissions(page::page_flags::EXECUTABLE);

    uint32_t result;
    uint32_t resume = 0;
    bool stop = 0;
    state r = {0, 0, 0, 0, 0, 0, 0};
    void* a[] = {&r.A, &r.B, &r.C, &r.D, &r.E, &r.F, &r.PC};
    void* params[] = {&resume, &stop, &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6]};
    test.function().apply(params, (void*)&result);
    cout << "Result: " << result << endl;
}
