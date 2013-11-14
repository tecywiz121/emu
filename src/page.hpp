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
#ifndef PAGE_HPP
#define PAGE_HPP 1

#include <cstdint>
#include <vector>
#include <memory>

#include "computer.hpp"
#include "page_function.hpp"

namespace tomato
{

class page
{
private:
    int _flags = 0;
    computer& _parent;
    std::vector<uint16_t> _memory;
    std::unique_ptr<page_function> _function;

    page(const page&);
    page(page&&);

public:
    page(computer& p, std::vector<uint16_t>::size_type size)
        : _parent(p), _memory(size)
    {}

    enum page_flags
    {
        NONE        = 0,
        READONLY    = 1,
        READWRITE   = 2,
        EXECUTABLE  = 3,

        USERSPACE   = 4,

        PRESENT     = 8,
    };

    computer& parent() { return _parent; }
    page_function& function() { return *_function; }

    void permissions(page_flags flags);

    bool readonly() const
    {
        return (_flags & page_flags::EXECUTABLE) == page_flags::READONLY;
    }

    bool readwrite() const
    {
        return (_flags & page_flags::EXECUTABLE) == page_flags::READWRITE;
    }

    bool executable() const
    {
        return (_flags & page_flags::EXECUTABLE) == page_flags::EXECUTABLE;
    }

    void userspace(bool v)
    {
        if (v) {
            _flags |= page_flags::USERSPACE;
        } else {
            _flags &= ~page_flags::USERSPACE;
        }
    }

    bool userspace() const
    {
        return _flags & page_flags::USERSPACE;
    }

    void present(bool v)
    {
        if (v) {
            _flags |= page_flags::PRESENT;
        } else {
            _flags &= ~page_flags::PRESENT;
        }
    }

    bool present() const
    {
        return _flags & page_flags::PRESENT;
    }

    std::vector<uint16_t>::size_type size() const
    {
        return _memory.size();
    }

    const std::vector<uint16_t>& memory() const
    {
        return _memory;
    }

    std::vector<uint16_t>& memory()
    {
        return _memory;
    }

};

}

#endif
