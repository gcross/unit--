//@+leo-ver=4-thin
//@+node:gcross.20101003140804.1347:@thin utility.cpp
//@@language cplusplus
//@@tabwidth -4
//@<< Licence >>
//@+node:gcross.20101003140804.1348:<< Licence >>
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
//@-node:gcross.20101003140804.1348:<< Licence >>
//@nl
//@<< Includes >>
//@+node:gcross.20101003140804.1349:<< Includes >>
#include <string>
#include <sstream>
#include <stdexcept>
#include "../include/unit--.hpp"
//@-node:gcross.20101003140804.1349:<< Includes >>
//@nl

//@+others
//@+node:gcross.20101003140804.1352:Utility functions
//@+node:gcross.20101003140804.1354:strTo
template <typename T>
inline T strTo(const std::string& str)
{
    std::istringstream ii(str);
    T result;
    if (ii >> result) return result;
    else throw std::invalid_argument("cannot convert (" + str + ")");
}
//@-node:gcross.20101003140804.1354:strTo
//@+node:gcross.20101003140804.1355:getNumber
// find sub string enclosed by < and >, starting at "end"
void getNumber(const std::string& message, unsigned& begin, unsigned& end)
{
    using std::string;
    begin = static_cast<unsigned>(message.find("<", end));
    assertTrue(string::npos != begin);
    ++begin;
    assertTrue(message.length() > begin);
    end = static_cast<unsigned>(message.find(">", begin));
    assertTrue(string::npos != end);
}
//@nonl
//@-node:gcross.20101003140804.1355:getNumber
//@-node:gcross.20101003140804.1352:Utility functions
//@+node:gcross.20101003140804.1350:Tests
testSuite(UtilitySuite) {

//@+others
//@+node:gcross.20101003140804.1351:AssertionInfoCase
// test if AssertionInfo transfer info correctly
testCase(AssertionInfoCase)
{
    {
        unit_minus::AssertionInfo info(true);
        assertTrue(true  == info.successful());
        assertTrue(false == info.hasMessage());
    }

    {
        unit_minus::AssertionInfo info(false);
        assertTrue(false == info.successful());
        assertTrue(false == info.hasMessage());
        assertTrue("" == info.message());
    }

    {
        const std::string message = "arBitRarY sTriNg";
        unit_minus::AssertionInfo info(false, message);
        assertTrue(false == info.successful());
        assertTrue(true  == info.hasMessage());
        assertTrue(message == info.message());
    }
}
//@nonl
//@-node:gcross.20101003140804.1351:AssertionInfoCase
//@+node:gcross.20101003140804.1356:EqualValueCase
// test if equalValueInfo give correct hint about failure
testCase(EqualValueCase)
{

    {
        const unsigned expected = 1;
        const unsigned actual = 2;
        unit_minus::AssertionInfo info(unit_minus::equalValueInfo(expected, actual));
        assertTrue(!info.successful());
        assertTrue( info.hasMessage());

        using std::string;
        string message = info.message();
        assertTrue(message.length() > 0);

        unsigned begin = 0;
        unsigned end = 0;
        getNumber(message, begin, end);
        assertTrue(expected == strTo<unsigned>(message.substr(begin, end - begin)));

        getNumber(message, begin, end);
        assertTrue(actual == strTo<unsigned>(message.substr(begin, end - begin)));
    }
}
//@-node:gcross.20101003140804.1356:EqualValueCase
//@+node:gcross.20101003140804.1357:CloseValueCase
// test if closeValueInfo give correct hint about failure
testCase(CloseValueCase)
{

    {
        const float expected = 0.0f + 3.5f - 1.9f;
        const double actual = 2.0;
        const double prec = 0.01;
        unit_minus::AssertionInfo info(unit_minus::closeValueInfo(expected, actual, prec));
        assertTrue(!info.successful());
        assertTrue( info.hasMessage());

        using std::string;
        string message = info.message();
        assertTrue(message.length() > 0);

        unsigned begin = 0;
        unsigned end = 0;
        getNumber(message, begin, end);
        const float expectedFromMessage = strTo<float>(message.substr(begin, end - begin));
        assertTrue(unit_minus::closeValueInfo(expected, expectedFromMessage, 0.001));

        getNumber(message, begin, end);
        const double actualFromMessage = strTo<double>(message.substr(begin, end - begin));
        assertTrue(unit_minus::closeValueInfo(actual, actualFromMessage, 0.001));

        getNumber(message, begin, end);
        const double precFromMessage = strTo<double>(message.substr(begin, end - begin));
        assertTrue(unit_minus::closeValueInfo(prec, precFromMessage, 0.001));
    }
}
//@nonl
//@-node:gcross.20101003140804.1357:CloseValueCase
//@-others

}
//@nonl
//@-node:gcross.20101003140804.1350:Tests
//@-others
//@-node:gcross.20101003140804.1347:@thin utility.cpp
//@-leo
