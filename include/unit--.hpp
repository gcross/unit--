//@+leo-ver=4-thin
//@+node:gcross.20090119092241.16:@thin unit--.hpp
//@@language C++
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

#ifndef unitmm_H_opt123
#define unitmm_H_opt123

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

//@+others
//@+node:gcross.20090119092241.39:class FailureInfo
// note failure information and transfer it out
class FailureInfo {
private:
    const char* m_file;
    int m_line;
    std::string m_desc;
public:
    FailureInfo(const char* filep, int linep, const std::string& descp)
        : m_file(filep), m_line(linep), m_desc(descp) {}
    const char* file() const { return m_file; }
    int line() const { return m_line; }
    const std::string& desc() const { return m_desc; }
};
//@nonl
//@-node:gcross.20090119092241.39:class FailureInfo
//@+node:gcross.20090119092241.17:namespace unit_minus
namespace unit_minus {

    class SuiteBase;
    class CaseBase;

    //@    @+others
    //@+node:gcross.20090119092241.29:Assertions
    //@+node:gcross.20090119092241.30:class AssertionInfo
    //--------------------------------------------------
    class AssertionInfo {
    private:
        bool m_successful;
        bool m_hasMessage;
        std::string m_message;

        std::string m_rawMessage;
    public:
        AssertionInfo(bool successp)
            : m_successful(successp), m_hasMessage(false) {}
        AssertionInfo(bool successp, const std::string& messagep)
            : m_successful(successp), m_hasMessage(true), m_message(messagep) {}
        bool successful() const { return m_successful; }
        operator bool () const { return successful(); }
        bool operator ! () const { return !successful(); }

        bool hasMessage() const { return m_hasMessage; }
        const std::string& message() const { return m_message; }

        const std::string& rawMessage() const { return m_rawMessage; }
        void rawMessage(const std::string& rawMess) { m_rawMessage = rawMess; }
    };
    //@nonl
    //@-node:gcross.20090119092241.30:class AssertionInfo
    //@+node:gcross.20090119092241.31:class AssertionChecker
    class AssertionChecker;
    // singleton, 7.1.2 - 4
    extern inline AssertionChecker*& currentChecker()
    {
        static AssertionChecker* s_ptr;
        return s_ptr;
    }
    class AssertionChecker {
        typedef AssertionChecker Self;
    public:
        static void reg(Self& checker) { currentChecker() = &checker; }
        static Self& get() { return *currentChecker(); }

        virtual void before(const char* file, int line) =0;
        virtual void after() =0;
        virtual void check(const AssertionInfo& info) =0;
    };
    //@nonl
    //@-node:gcross.20090119092241.31:class AssertionChecker
    //@+node:gcross.20090119092241.32:struct CheckerGuard
    // make sure before() and after() are called
    struct CheckerGuard {
        CheckerGuard(const char* file, int line)
            { AssertionChecker::get().before(file, line); }
        ~CheckerGuard() { AssertionChecker::get().after(); }
    };
    //@nonl
    //@-node:gcross.20090119092241.32:struct CheckerGuard
    //@+node:gcross.20090119092241.33:checking functions
    //@+node:gcross.20101001160513.1303:equal value
    template <typename T1, typename T2>
    inline AssertionInfo equalValueInfo(const T1& expected, const T2& actual)
    {
        if (expected == actual) return AssertionInfo(true);

        std::ostringstream oo;
        oo << "expected <" << expected << ">, but was <" << actual << ">";
        return AssertionInfo(false, oo.str());
    }
    //@nonl
    //@-node:gcross.20101001160513.1303:equal value
    //@+node:gcross.20101001160513.1306:greater or equal value
    template <typename T1, typename T2>
    inline AssertionInfo greaterOrEqualValueInfo(const T1& x, std::string x_name, const T2& y, std::string y_name)
    {
        if (x >= y) return AssertionInfo(true);

        std::ostringstream oo;
        oo << "have " << x_name << " <" << x << "> < " << y_name << " <" << y << ">";
        return AssertionInfo(false, oo.str());
    }
    //@nonl
    //@-node:gcross.20101001160513.1306:greater or equal value
    //@+node:gcross.20101001160513.1308:less or equal value
    template <typename T1, typename T2>
    inline AssertionInfo lessOrEqualValueInfo(const T1& x, std::string x_name, const T2& y, std::string y_name)
    {
        if (x <= y) return AssertionInfo(true);

        std::ostringstream oo;
        oo << "have " << x_name << " <" << x << "> < " << y_name << " <" << y << ">";
        return AssertionInfo(false, oo.str());
    }
    //@nonl
    //@-node:gcross.20101001160513.1308:less or equal value
    //@+node:gcross.20101001160513.1304:close value
    template <typename T1, typename T2, typename T3>
    inline AssertionInfo closeValueInfo(const T1& expected, const T2& actual, const T3& precision)
    {
        if (
            (expected + precision >= actual  )
         && (actual   + precision >= expected)
        ) return AssertionInfo(true);

        std::ostringstream oo;
        oo << "expected <" << expected << ">, but was <" << actual << ">, which is not close enough with precision <" << precision << ">";
        return AssertionInfo(false, oo.str());
    }
    //@-node:gcross.20101001160513.1304:close value
    //@-node:gcross.20090119092241.33:checking functions
    //@-node:gcross.20090119092241.29:Assertions
    //@+node:gcross.20090418183921.7:Runners
    //@+node:gcross.20090119092241.18:class Runner
    // log entire test process, providing simple control
    class Runner {
    public:
        virtual ~Runner() {}
        // enter() & exit() are called for every test suite
        virtual void enter(SuiteBase&) =0;
        virtual void exit(SuiteBase&) =0;
        // check() is called for every test case
        virtual void check(CaseBase&) =0;
    };
    //@-node:gcross.20090119092241.18:class Runner
    //@+node:gcross.20090418183921.8:class ANSIRunner
    // test all cases, and output error message to std::cout
    class ANSIRunner: public Runner {

        enum {INDENT_SIZE = 4};

    private:
        typedef ANSIRunner Self;
        unsigned m_failureCount;
        unsigned m_totalCount;
        unsigned m_indent;

        class Checker: public AssertionChecker {
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
        std::string m_errorFormat;
    public:
        ANSIRunner(const std::string& errorFormat)
            : m_failureCount(0), m_totalCount(0), m_indent(0)
        {
            unit_minus::AssertionChecker::reg(m_checker);
            m_errorFormat = errorFormat;
        }
        virtual void enter(SuiteBase& suite);
        virtual void exit(SuiteBase&);
        virtual void check(CaseBase& aCase);
        virtual void fail(CaseBase& aCase, FailureInfo& info);
        std::string errorMessage(FailureInfo& info) const;
        virtual void pass(CaseBase&);
        bool ok() const;
        void printSummary() const;
    };
    //@nonl
    //@-node:gcross.20090418183921.8:class ANSIRunner
    //@-node:gcross.20090418183921.7:Runners
    //@+node:gcross.20090119092241.19:classs TestBase
    //--------------------------------------------------
    // base class of tests
    class TestBase {
        // tests will chain up as a linked list to avoid dependence on
        // dynamic memory allocation init
        virtual TestBase*& nextPointer() = 0;
        friend class SuiteBase;
    public:
        virtual ~TestBase() {}
        virtual void run(Runner&) =0;
        virtual const char* caption() const =0;
    };
    //@nonl
    //@-node:gcross.20090119092241.19:classs TestBase
    //@+node:gcross.20090119092241.20:Suites
    //@+node:gcross.20090119092241.21:class SuiteBase
    //--------------------------------------------------
    struct SuiteState {
        bool added;
        TestBase* tests;
        TestBase* nextPointer;
    };
    // abstract class
    // test suite defination
    class SuiteBase: public TestBase {
    protected:
        SuiteState* m_state;
    private:
        // header of linked list
        TestBase*& tests()
        {
            return m_state->tests;
        }
        virtual TestBase*& nextPointer()
        {
            return m_state->nextPointer;
        }
    public:
        void setState(SuiteState* statep)
        {
            m_state = statep;
        }
        virtual void run(Runner& log);
        bool add(TestBase& test)
        {
            test.nextPointer() = tests();
            tests() = &test;
            return true;
        }
    };
    //@-node:gcross.20090119092241.21:class SuiteBase
    //@+node:gcross.20090119092241.22:function suiteInstance
    // implement singleton work
    template <typename ThisSuite>
    extern inline ThisSuite& suiteInstance()
    {
        static SuiteState state = { false, 0, 0 };
        static ThisSuite s_inst;
        s_inst.setState(&state);
        return s_inst;
    }
    //@nonl
    //@-node:gcross.20090119092241.22:function suiteInstance
    //@+node:gcross.20090119092241.23:class Suite
    // abstract class
    template <class ThisSuite, class UpperSuite>
    class Suite: public SuiteBase {
        typedef Suite Self;
    public:
        bool addToUpper()
        {
            // add will be performed only once
            return m_state->added = m_state->added || suiteInstance<UpperSuite>().add(*this);
        }
    };
    //@nonl
    //@-node:gcross.20090119092241.23:class Suite
    //@+node:gcross.20090119092241.24:class Root (suite)
    // the top most test suite
    // every test case should be attached to Root directly or indirectly
    // it's add to upper is not called, OR there will be a INFINITE LOOP
    class Root: public Suite<Root, Root> {
    public:
        virtual const char* caption() const
        {
            return "all tests";
        }
    };
    //@-node:gcross.20090119092241.24:class Root (suite)
    //@-node:gcross.20090119092241.20:Suites
    //@+node:gcross.20090119092241.25:Test Cases
    //@+node:gcross.20090119092241.26:class CaseBase
    //--------------------------------------------------
    struct CaseState {
        bool added;
        TestBase* nextPointer;
    };
    // test work implementation
    // abstract class
    class CaseBase: public TestBase {
    protected:
        CaseState* m_state;
    private:
        virtual TestBase*& nextPointer()
        {
            return m_state->nextPointer;
        }
    public:
        virtual void test() =0;
        virtual const char* file() const =0;
        virtual int line() const =0;
    public:
        void setState(CaseState* statep)
        {
            m_state = statep;
        }
        virtual void run(Runner& log)
        {
            log.check(*this);
        }
    };
    //@nonl
    //@-node:gcross.20090119092241.26:class CaseBase
    //@+node:gcross.20090119092241.27:function caseInstance
    // singleton work
    template <typename ThisCase>
    extern inline ThisCase& caseInstance()
    {
        static CaseState state = { false, 0 };
        static ThisCase s_inst;
        s_inst.setState(&state);
        return s_inst;
    }
    //@nonl
    //@-node:gcross.20090119092241.27:function caseInstance
    //@+node:gcross.20090119092241.28:class Case
    // abstract class
    // see Suite
    template <class ThisCase, class UpperSuite>
    class Case: public CaseBase {
    private:
        typedef Case Self;
    public:
        bool addToUpper()
        {
            return m_state->added = m_state->added || suiteInstance<UpperSuite>().add(*this);
        }
    };
    //@-node:gcross.20090119092241.28:class Case
    //@-node:gcross.20090119092241.25:Test Cases
    //@+node:gcross.20090119092241.34:miscelleneous
    int printHelp(const std::string& execFile);
    int listCases();
    std::string getFormat(const std::string& style);
    int runTest(const std::string& errorFormat);
    int main(int argc, char* argv[]);
    std::string withoutUnderscores(const std::string& in);
    //@nonl
    //@-node:gcross.20090119092241.34:miscelleneous
    //@-others


} // namespace unit_minus
//@-node:gcross.20090119092241.17:namespace unit_minus
//@+node:gcross.20090119092241.35:Macros
//@+node:gcross.20090119092241.36:suite macros
// wrapper code for Suite<>
// a test suite should be a Suite<>
#define testSuitePrefix(thisSuite, upperSuite, prefix)                        \
namespace thisSuite {                                                         \
class thisSuite: public unit_minus::Suite<thisSuite, upperSuite> {            \
    public: virtual const char* caption() const { return #thisSuite; }        \
};                                                                            \
const bool prefix##thisSuite                                                  \
    = unit_minus::suiteInstance<thisSuite>().addToUpper();                    \
}                                                                             \
namespace thisSuite

#define subSuite(thisSuite, upperSuite)                                       \
testSuitePrefix(thisSuite, upperSuite, added_)

#define testSuite(thisSuite) subSuite(thisSuite, unit_minus::Root)
//@nonl
//@-node:gcross.20090119092241.36:suite macros
//@+node:gcross.20090119092241.37:test case macros
// wrapper code for Case<>
// a test case should be a Case<>
#define testCasePrefix(thisCase, upperSuite, prefix)                          \
class thisCase: public unit_minus::Case<thisCase, upperSuite> {               \
public:                                                                       \
    virtual const char* caption() const { return #thisCase; }                 \
private:                                                                      \
    virtual const char* file() const { return __FILE__; }                     \
    virtual int line() const { return __LINE__; }                             \
    virtual void test();                                                      \
};                                                                            \
const bool prefix##thisCase                                                   \
    = unit_minus::caseInstance<thisCase>().addToUpper();                      \
inline void thisCase::test()

#define testCase(thisCase, upperSuite)                                        \
testCasePrefix(thisCase, upperSuite, added_)
//@nonl
//@-node:gcross.20090119092241.37:test case macros
//@+node:gcross.20090119092241.38:assertion macros
//@+node:gcross.20101001160513.1300:assertTrue
// test assertion
// param: expression to test
// true for pass, false for failure
#define assertTrue(opt_expression)                                            \
{                                                                             \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                \
    unit_minus::AssertionInfo opt_info(opt_expression);                       \
    opt_info.rawMessage("Assertion failed: <" #opt_expression ">");           \
    unit_minus::AssertionChecker::get().check(opt_info);                      \
}
//@-node:gcross.20101001160513.1300:assertTrue
//@+node:gcross.20101001160513.1301:assertEqual
// test assertion
// param: values whose equality is to be tested
#define assertEqual(A,B)                                                      \
{                                                                             \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                \
    unit_minus::AssertionInfo opt_info = unit_minus::equalValueInfo(A,B);     \
    unit_minus::AssertionChecker::get().check(opt_info);                      \
}
//@nonl
//@-node:gcross.20101001160513.1301:assertEqual
//@+node:gcross.20101001221615.1310:assertAlmostEqual
// test assertion
// param: values whose equality is to be tested
#define assertAlmostEqual(A,B,C)                                                      \
{                                                                             \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                \
    unit_minus::AssertionInfo opt_info = unit_minus::closeValueInfo(A,B,C);     \
    unit_minus::AssertionChecker::get().check(opt_info);                      \
}
//@nonl
//@-node:gcross.20101001221615.1310:assertAlmostEqual
//@+node:gcross.20101001160513.1310:assertGreaterOrEqual
// test assertion
// param: values which are to be compared
#define assertGreaterOrEqual(A,B)                                                  \
{                                                                                  \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                     \
    unit_minus::AssertionInfo opt_info = unit_minus::greaterOrEqualValueInfo(A, #A ,B, #B ); \
    unit_minus::AssertionChecker::get().check(opt_info);                           \
}

#define assertGE(A,B) assertGreaterOrEqual(A,B)
//@-node:gcross.20101001160513.1310:assertGreaterOrEqual
//@+node:gcross.20101001160513.1312:assertLessOrEqual
// test assertion
// param: values whose equality is to be tested
#define assertLessOrEqual(A,B)                                                  \
{                                                                               \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                  \
    unit_minus::AssertionInfo opt_info = unit_minus::lessOrEqualValueInfo(A, #A, B, #B ); \
    unit_minus::AssertionChecker::get().check(opt_info);                        \
}

#define assertLE(A,B) assertLessOrEqual(A,B)
//@-node:gcross.20101001160513.1312:assertLessOrEqual
//@+node:gcross.20101001160513.1302:assertNoArrive
// test assertion
// assume failure when executed
// param: error message
#define assertNoArrive(opt_message)                                           \
{                                                                             \
    unit_minus::CheckerGuard opt_tempGard(__FILE__, __LINE__);                \
    unit_minus::AssertionInfo opt_info(false, (opt_message));                 \
    opt_info.rawMessage(opt_message);                                         \
    unit_minus::AssertionChecker::get().check(opt_info);                      \
}
//@nonl
//@-node:gcross.20101001160513.1302:assertNoArrive
//@-node:gcross.20090119092241.38:assertion macros
//@+node:gcross.20090119092241.40:default main
#define useDefaultMain int main(int argc, char* argv[]) { unit_minus::main(argc,argv); }
//@-node:gcross.20090119092241.40:default main
//@-node:gcross.20090119092241.35:Macros
//@-others


#endif // unitmm_H_opt123
//@-node:gcross.20090119092241.16:@thin unit--.hpp
//@-leo
