//@+leo-ver=5-thin
//@+node:gcross.20101003140804.1341: * @thin examples.cpp
//@@language cplusplus
//@@tabwidth -4
//@+<< Licence >>
//@+node:gcross.20101003140804.1342: ** << Licence >>
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
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//@@c
//@-<< Licence >>

//@+at
// This file provides examples of how to use the unit-- test framework.
// 
// First, one needs to include the header file:
//@@c
#include "../include/unit--.hpp"

//@+at
// One defines suites using the testSuite macro:
//@@c
testSuite(IllustrationSuite) {

//@+at
// To define a new suite that is a child of another suite, one may use the subSuite macro.
//@@c
subSuite(EmptySuite) {}

//@+at
// Test cases are defined as children of test suites.
//@@c
testCase(BoolExpression)
{
    // The assertTrue macro performas a general assertion;  the test fails if the
    // assertion is true
    assertTrue(1 == 1u)

    // Since failure prints the expression inside the macro, one can use the trick
    // illustrated as follows to display a descriptive message when the tests fails:
    assertTrue(true && "signed and unsigned int shall be equal");

    // For complicated expressions, it may prove useful to store the expression in
    // a descriptive variable name in order to improve the clarify of the
    // failure message.
    bool someComplicatedExpression = (1 == 2 || (1 == 1 && 0 == 0) || false);

    // When we reach the end of the test, if we have seen no failures then we asseme
    // that the test has succeeded.
}

//@+at
// This is an example of another test attached to the same suite.
//@@c
testCase(NoArriveCase)
{
    try {
        // Sometimes we want to test that an exception was thrown, such as in
        // the following code:
        int aPositiveValue = -1;
        if(aPositiveValue <= 0) throw aPositiveValue;

        // The assertNoArrive macro fails the test with the given message if
        // the flow of execution reaches this point.
        assertNoArrive("an invalid parameter is not rejected");
    }
    // try to use the *exact* type of exception
    // do NOT use catch (...), or some failure cannot be caught by unit--
    catch (int) {
    }
}

//@+at
// For floating point values, one should use the assertClose macro to perform an approximate comparison.
//@@c
testCase(FloatingPoint)
{

    float expectedValue = 1.234f;
    float errorLimit = 0.001f;
    float value = 1.03f + 0.204f;

    assertAlmostEqual(expectedValue, value, errorLimit);
    assertAlmostEqual(1.66666666, 5.0 / 3.0, 0.001);
}

//@+at
// unit-- does not provide special functionality for test fixtures, but fortunately C++'s semantics make it easy to get this functionality using native classes, as illustrated in the following example.
//@@c
struct MyFixture {
    // variables that needed by some test case
    double x;
    double y;
    unsigned n;

    // constructor for setUp() job
    MyFixture()
    {
        n = 10;
        x = 1.3;
        y = x;
        for (unsigned i = 0; i < n; ++i) {
            y += y;
        }
    }
    // destructor for tearDown() job
    ~MyFixture()
    {
        // what ever you need
    }
};

testCase(FixtureCase)
{
    // declear a local variable to invoke the constructor for setUp job
    // and (automatically) register the destructor for tearDown
    MyFixture f;

    using namespace unit_minus;

    double factor = 1.0;
    for (unsigned i = 0; i < f.n; ++i) {
        factor *= 2;
    }
    // check if different algorithm get same answer
    assertAlmostEqual(f.y, f.x * factor, 0.01);
}

} // end test suite
//@-leo
