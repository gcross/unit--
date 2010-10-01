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

#include <vector>
#include <numeric>
#include <algorithm>
#include "../include/unit--.h"

testSuite(TemplateSuite)

template <typename T>
void testAlgorithms()
{
    using namespace std;
    using namespace unit_minus;
    vector<T> ve(100, 1);
    partial_sum(ve.begin(), ve.end(), ve.begin());

    assertTrue(ve.size() > 0);
    assertTrue(1 == ve[0]);
    for (unsigned i = 1; i < ve.size(); ++i) {
        assertTrue(equalValueInfo(ve[i - 1] + 1, ve[i]));
    }
}

namespace {
testCase(IntCase, TemplateSuite)
{
    testAlgorithms<int>();
}

testCase(UnsignedCase, TemplateSuite)
{
    testAlgorithms<unsigned>();
}

} // namespace
