//@+leo-ver=5-thin
//@+node:gcross.20090119092241.4: * @thin unit--.cpp
//@@language cplusplus
//@@tabwidth -4

//@+<< License >>
//@+node:gcross.20101203210234.1954: ** << License >>
//@+at
// unit--, a simple and easy-to-use unit test aid for C++
// Copyright (C) 2005~2006  Tsong Chong
// birdiez@126.com
// 
//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//@-<< License >>

#include <iostream>
#include <string>
#include <ctime>
#include "unit--.hpp"

namespace unit_minus {

using namespace std;

//@+others
//@+node:gcross.20090119092241.6: ** Runners
//@+node:gcross.20090119092241.7: *3* class ANSIRunner



//@+node:gcross.20101003152339.1348: *4* check
void ANSIRunner::check(CaseBase& aCase)
{
    try {
        cout << string(INDENT_SIZE * m_indent, ' ');
        cout << "\033[1;33m" << withoutUnderscores(aCase.caption()) << "... \033[0m" << flush;
        aCase.test();
        pass(aCase);
        return;
    }
    // assertion failed
    catch (FailureInfo& info) {
        fail(aCase, info);
        return;
    }
    // some kind of unexpected exception
    catch (exception& e) {
        ostringstream mess;
        mess << "unexpected exception with description:\n";
        mess << e.what();
        FailureInfo info(aCase.file(), aCase.line(), mess.str());
        fail(aCase, info);
        return;
    }
    catch (...) {
        ostringstream mess;
        mess << "unexpected unknown exception";
        FailureInfo info(aCase.file(), aCase.line(), mess.str());
        fail(aCase, info);
        return;
    }
}
//@+node:gcross.20101003152339.1346: *4* enter
void ANSIRunner::enter(SuiteBase& suite) {
    cout << string(INDENT_SIZE * m_indent, ' ');
    cout << "\033[43m" << withoutUnderscores(suite.caption()) << ":\033[0m" << endl;
    ++m_indent;
}
//@+node:gcross.20101003152339.1350: *4* errorMessage
string ANSIRunner::errorMessage(FailureInfo& info) const
{
    ostringstream oo;
    for (string::size_type i = 0; i < m_errorFormat.length(); ++i) {
        if ('_' != m_errorFormat[i]) {
            oo << m_errorFormat[i];
            continue;
        }

        ++i;
        if (m_errorFormat.length() == i) break;
        if ('f' == m_errorFormat[i]) oo << info.file();
        else if ('l' == m_errorFormat[i]) oo << info.line();
        else if ('m' == m_errorFormat[i]) oo << info.desc();
        else oo << m_errorFormat[i];
    }
    return oo.str();
}
//@+node:gcross.20101003152339.1347: *4* exit
void ANSIRunner::exit(SuiteBase&) {
    --m_indent;
}
//@+node:gcross.20101003152339.1349: *4* fail
// a test case failed
void ANSIRunner::fail(CaseBase& aCase, FailureInfo& info)
{
    cout << "\033[1;31m FAIL :-( \033[0m" << endl;
    cout << string(INDENT_SIZE * (m_indent+1), ' ');
    cout << "\033[1;31m" << errorMessage(info) << "\033[0m" << endl;

    ++m_failureCount;
    ++m_totalCount;
}
//@+node:gcross.20101003152339.1352: *4* ok
bool ANSIRunner::ok() const
{
    return 0 == m_failureCount;
}
//@+node:gcross.20101003152339.1351: *4* pass
void ANSIRunner::pass(CaseBase&)
{
    ++m_totalCount;
    cout << "\033[1;32m PASS :-) \033[0m" << endl;
}
//@+node:gcross.20101003152339.1353: *4* printSummary
void ANSIRunner::printSummary() const
{
    cout << "\n";
    if (0 == m_failureCount) {
        cout << "OK" << endl;
    }
    else {
        cout << m_failureCount << " case";
        if (m_failureCount > 1) cout << "s";
        cout << " failed" << endl;
    }

    cout << "Total " << m_totalCount << " test case";
    if (m_totalCount > 1) cout << "s";
    cout << endl;
}
//@+node:gcross.20090119092241.8: *3* class StdRunner
// test all cases, and output error message to cout
class StdRunner: public unit_minus::Runner {
private:
    typedef StdRunner Self;
    unsigned m_failureCount;
    unsigned m_totalCount;

    class Checker: public unit_minus::AssertionChecker {
        const char* m_file;
        int m_line;
    public:
        virtual void before(const char* file, int line)
        {
            m_file = file;
            m_line = line;
        }
        virtual void after() {}
        virtual void check(const unit_minus::AssertionInfo& info)
        {
            if (info.successful()) return;
            throw FailureInfo(
                m_file, m_line,
                (info.hasMessage() ? info.message() : info.rawMessage()));
        }
    };

    Checker m_checker;
    string m_errorFormat;
public:
    StdRunner(const string& errorFormat)
        : m_failureCount(0), m_totalCount(0)
    {
        unit_minus::AssertionChecker::reg(m_checker);
        m_errorFormat = errorFormat;
    }
    // do not care about suites
    virtual void enter(SuiteBase&) {}
    virtual void exit(SuiteBase&) {}

    virtual void check(CaseBase& aCase)
    {
        try {
            aCase.test();
            pass(aCase);
            return;
        }
        // assertion failed
        catch (FailureInfo& info) {
            fail(aCase, info);
            return;
        }
        // some kind of unexpected exception
        catch (exception& e) {
            ostringstream mess;
            mess << "unexpected exception with description:\n";
            mess << e.what();
            FailureInfo info(aCase.file(), aCase.line(), mess.str());
            fail(aCase, info);
            return;
        }
        catch (...) {
            ostringstream mess;
            mess << "unexpected unknown exception";
            FailureInfo info(aCase.file(), aCase.line(), mess.str());
            fail(aCase, info);
            return;
        }
    }

    // a test case failed
    virtual void fail(CaseBase& aCase, FailureInfo& info)
    {
        cout << "X\n";
        cout << aCase.caption() << "\n";
        cout << errorMessage(info) << "\n";

        ++m_failureCount;
        ++m_totalCount;
    }
    string errorMessage(FailureInfo& info) const
    {
        ostringstream oo;
        for (string::size_type i = 0; i < m_errorFormat.length(); ++i) {
            if ('_' != m_errorFormat[i]) {
                oo << m_errorFormat[i];
                continue;
            }

            ++i;
            if (m_errorFormat.length() == i) break;
            if ('f' == m_errorFormat[i]) oo << info.file();
            else if ('l' == m_errorFormat[i]) oo << info.line();
            else if ('m' == m_errorFormat[i]) oo << info.desc();
            else oo << m_errorFormat[i];
        }
        return oo.str();
    }

    // a test case passed
    virtual void pass(CaseBase&)
    {
        ++m_totalCount;
        cout << "." << flush;
    }

    bool ok() const
    {
        return 0 == m_failureCount;
    }
    // print summery of whole test
    void printSummary() const
    {
        cout << "\n";
        if (0 == m_failureCount) {
            cout << "OK" << endl;
        }
        else {
            cout << m_failureCount << " case";
            if (m_failureCount > 1) cout << "s";
            cout << " failed" << endl;
        }

        cout << "Total " << m_totalCount << " test case";
        if (m_totalCount > 1) cout << "s";
        cout << endl;
    }
};
//@+node:gcross.20090119092241.9: *3* class ListBuilder
// list all test cases with a tree structure, but don't do any test
class ListBuilder: public unit_minus::Runner {
private:
    enum {INDENT_SIZE = 4};
    unsigned m_indent;
    typedef ListBuilder Self;
public:
    ListBuilder(): m_indent(0) {}
    virtual void enter(SuiteBase& suite)
    {
        cout << string(INDENT_SIZE * m_indent, ' ');
        cout << suite.caption() << endl;
        ++m_indent;
    }
    virtual void exit(SuiteBase&)
    {
        --m_indent;
    }

    virtual void check(CaseBase& ca)
    {
        cout << string(INDENT_SIZE * m_indent, ' ');
        cout << ca.caption() << "\n";
    }
};

//@+node:gcross.20090418183921.17: ** Suites
//@+node:gcross.20090418183921.18: *3* class SuiteBase
void SuiteBase::run(Runner& log)
{
    log.enter(*this);
    vector<TestBase*> tests_to_run;
    for (TestBase* p = tests(); p != 0; p = p->nextPointer()) {
        tests_to_run.push_back(p);
    }
    for (vector<TestBase*>::reverse_iterator pp = tests_to_run.rbegin(); pp != tests_to_run.rend(); ++pp) {
        (*pp)->run(log);
    }
    log.exit(*this);
}
//@+node:gcross.20101003152339.1344: ** Functions
//@+node:gcross.20090119092241.13: *3* getFormat
string getFormat(const string& style)
{
    if ("gnu" == style) return "_f:_l: _m";
    if ("vc"  == style) return "_f(_l) : error: _m";

    const string format = "format";
    if (style.length() > format.length()
        && style.substr(0, format.length()) == format
    ) {
        return style.substr(format.length(), string::npos);
    }
    return getFormat("gnu");
}
//@+node:gcross.20090119092241.12: *3* listCases
int listCases()
{
    ListBuilder listBuilder;
    suiteInstance<Root>().run(listBuilder);
    return 0;
}
//@+node:gcross.20090119092241.15: *3* main
int main(int argc, char* argv[])
{
    if (argc < 2) {
        return runTest(getFormat("gnu"));
    }

    if (argc > 2 || string(argv[1]) == "-h" || string(argv[1]) == "--help") {
        return printHelp(argv[0]);
    }

    if (string(argv[1]) == "-l" || string(argv[1]) == "--list") {
        return listCases();
    }

    return runTest(getFormat(argv[1]));
}
//@+node:gcross.20090119092241.11: *3* printHelp
int printHelp(const string& execFile)
{
    cout
        << "usage:\n"
        << execFile << " [ <style> | -h | -l ]\n"
        << "\n"
        << "<style>       style of assertion failure report\n"
        << "    gnu       for GNU Emacs (default)\n"
        << "    vc        for MS Visual C++\n"
        << "    format\"<format string>\"\n"
        << "              there is no space between format and <format string>\n"
        << "              user defined style, there are some conversions available:\n"
        << "              _f for source file name,\n"
        << "              _l (lower case L) for line number, and\n"
        << "              _m for a (perhaps) descriptive message\n"
        << "    example:  gnu style is equivalent to format\"_f:_l: _m\"\n"
        << "\n"
        << "-h, --help    print this help message\n"
        << "-l, --list    list all test cases and suites in tree structure\n"
        << flush;
    return 0;
}
//@+node:gcross.20090119092241.14: *3* runTest
int runTest(const string& errorFormat)
{
    ANSIRunner runner(errorFormat);
    time_t t0 = time(0);
    suiteInstance<Root>().run(runner);
    double period = difftime(time(0), t0);
    runner.printSummary();
    cout << difftime(time(0), t0) << " sec";
    if (period > 1.0) cout << "s";
    cout <<"." << endl;
    return runner.ok() ? 0 : 1;
}
//@+node:gcross.20101003152339.1354: *3* withoutUnderscores
string withoutUnderscores(const string& in) {
    string out = in;
    for(string::iterator c_ptr = out.begin(); c_ptr < out.end(); ++c_ptr) {
        if(*c_ptr == '_') (*c_ptr) = ' ';
    }
    string::iterator c_ptr;
    for(c_ptr = --out.end(); *c_ptr == ' '; --c_ptr) ;
    ++c_ptr;
    out.erase(c_ptr,out.end());
    for(c_ptr = out.begin(); *c_ptr == ' '; ++c_ptr) ;
    out.erase(out.begin(),c_ptr);
    return out;
}
//@-others

}



//@-leo
