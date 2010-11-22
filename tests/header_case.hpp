//@+leo-ver=4-thin
//@+node:gcross.20101003140804.1333:@thin header_case.hpp
//@@language cplusplus
//@@tabwidth -4
//@<< Licence >>
//@+node:gcross.20101003140804.1334:<< Licence >>
//@+at
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
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
// USA
//@-at
//@@c
//@nonl
//@-node:gcross.20101003140804.1334:<< Licence >>
//@nl

#ifndef header_case_H_opt123
#define header_case_H_opt123

//@<< Includes >>
//@+node:gcross.20101003140804.1335:<< Includes >>
#include "../include/unit--.hpp"
//@-node:gcross.20101003140804.1335:<< Includes >>
//@nl

testSuite(HeaderSuite) {

class CountLog: public unit_minus::Runner {
    unsigned m_count;
public:
    CountLog(): m_count(0) {}
    unsigned count() { return m_count; }
    virtual void enter(unit_minus::SuiteBase&) {}
    virtual void exit(unit_minus::SuiteBase&) {}
    virtual void check(unit_minus::CaseBase&) { ++m_count; }
};

// this case is implemented in a header file,
// which would be included by 2 source files
// the case should be counted only once
testCase(OnceCase)
{
    using namespace unit_minus;
    CountLog cl;
    suiteInstance<HeaderSuite>().run(cl);
    assertTrue(equalValueInfo(1u, cl.count()));
}

}

#endif // header_case_H_opt123
//@-node:gcross.20101003140804.1333:@thin header_case.hpp
//@-leo
