// unit--, a simple and easy-to-use unit test aid for C++
// Copyright (C) 2005~2006  Tsong Chong
// birdiez@126.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "../include/unit--.h"
#include "header_case.h"

testSuite(StaticLink1)

namespace {
    unsigned i = 0;
    unsigned x()
    {
        return 2;
    }
} // namespace

static unsigned y()
{
    return 2;
}

namespace {

testCase(IncStatic2, StaticLink1)
{
    using namespace unit_minus;
    ++i;
    // varibles in anonymous namespace cannot be seen by other files
    assertTrue(equalValueInfo(1u, i));
    assertTrue(2 == x() && "functions in anonymous namespace cannot be redefined from other files");
    assertTrue(2 == y() && "static function has no external linkage");
}

} // namespace
