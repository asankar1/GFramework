#include <iostream>
#ifdef _WINDOWS
#include <conio.h>
#endif
#include <typeindex>
#include <vector>
#include <cctype>   //isupper, islower
#include <map>
#include "gtest/gtest.h"
#include "gvariant_test.h"
#include "gproperty_test.h"
#include "greflection_test.h"
#include "gserialization_test.h"
#include "gscript_test.h"

#include <Node.h>

using namespace std;
using namespace GFramework;

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using ::testing::TestSuite;

namespace testing {
    namespace internal {
        extern bool ShouldUseColor(bool stdout_is_tty);
    }
}

namespace {
    // Provides alternative output mode which produces minimal amount of
    // information about tests.
    class CustomPrinter : public EmptyTestEventListener {
    public:
        CustomPrinter() {
#if GTEST_OS_WINDOWS_MOBILE || GTEST_OS_ZOS || GTEST_OS_IOS || \
    GTEST_OS_WINDOWS_PHONE || GTEST_OS_WINDOWS_RT || defined(ESP_PLATFORM)
            use_color = false;
#else
            use_color = ::testing::internal::ShouldUseColor(::testing::internal::posix::IsATTY(::testing::internal::posix::FileNo(stdout)) != 0);
#endif  // GTEST_OS_WINDOWS_MOBILE || GTEST_OS_ZOS
        }
    private:
        bool use_color;
        enum Color {
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Magenta,
            Cyan,
            White,
            Reset,
        };

        const char* GetColor(Color clr)
        {
            if (!use_color)
                return "";

            switch (clr)
            {
            case Black: 	return "\u001b[90m";
            case Red: 		return "\u001b[91m";
            case Green: 	return "\u001b[92m";
            case Yellow:	return "\u001b[93m";
            case Blue:  	return "\u001b[94m";
            case Magenta:	return "\u001b[95m";
            case Cyan:  	return "\u001b[96m";
            case White:  	return "\u001b[97m";
            case Reset:  	return "\u001b[0m";
            }

            return "";
        }
        // Called before any test activity starts.
        void OnTestProgramStart(const UnitTest&  unit_test ) override {
            fprintf(stdout, "%sRunning %d test cases from %d test suites%s\n", GetColor(Color::White), unit_test.test_case_to_run_count(), unit_test.test_suite_to_run_count(), GetColor(Color::Reset));
            fflush(stdout);
        }

        void OnEnvironmentsSetUpStart(const UnitTest& /*unit_test*/) override {
            fprintf(stdout, "Global test environment set-up\n");
            fflush(stdout);
        }

        void OnTestSuiteStart(const TestSuite& test_suite) override {
            std::string suitname(test_suite.name());
            auto pos = suitname.find("$$");
            if (pos != std::string::npos) {
                suitname = suitname.replace(pos, 2, "::");
            }
            fprintf(stdout, "\n");
            fprintf(stdout, "%s[ TEST SUITE ]%s %s ( %d tests )\n", GetColor(Color::Cyan) , GetColor(Color::Reset), suitname.c_str(), test_suite.test_to_run_count() );
            fflush(stdout);
        }

        // Called after a test ends.
        void OnTestEnd(const TestInfo& test_info) override {
            const char* pass = "[    PASS    ]";
            const char* fail = "[====FAIL====]";
            const char* skip = "[    SKIP    ]";
            const char* result = pass;
            auto test_name = test_info.name();
            std::string pretty_test_name = "";
            pretty_test_name += test_name;
#if 0   //TODO: to be improved
            for (int i = 1; i < strlen(test_name); i++)
            {
                if (std::isupper(test_name[i]))
                {
                    if (std::islower(test_name[i-1]))
                    {
                        pretty_test_name += ' ';
                    }
                }
                pretty_test_name += test_name[i];
            }
#endif
            if (test_info.result()->Failed()){
                result = fail;
                fprintf(stdout, "%s%s%s Test: %s (%d ms)\n", GetColor(Color::Red),result, GetColor(Color::Reset), pretty_test_name.c_str(), test_info.result()->elapsed_time());
            }
            else if (test_info.result()->Skipped()){
                result = skip;
                fprintf(stdout, "%s%s%s Test: %s (%d ms)\n", GetColor(Color::Yellow), result, GetColor(Color::Reset), pretty_test_name.c_str(), test_info.result()->elapsed_time());
            }
            else
            {
                result = pass;
                fprintf(stdout, "%s%s%s Test: %s (%d ms)\n", GetColor(Color::Green), result, GetColor(Color::Reset), pretty_test_name.c_str(), test_info.result()->elapsed_time());
            }
            
            fflush(stdout);
        }

        void OnTestSuiteEnd(const TestSuite& test_suite) override {
            fprintf(stdout, "%s[---RESULT---]%s Pass: %d, Fail: %d, Skip: %d, Time: %d ms\n", \
                    GetColor(Color::Cyan), GetColor(Color::Reset), test_suite.successful_test_count(), test_suite.failed_test_count(), \
                    test_suite.skipped_test_count(), test_suite.elapsed_time());
            fprintf(stdout, "\n");
            fflush(stdout);
        }

        void OnEnvironmentsTearDownEnd(const UnitTest& /*unit_test*/) override {
            fprintf(stdout, "Global test environment tear-down\n");
            fflush(stdout);
        }

        // Called after all test activities have ended.
        void OnTestProgramEnd(const UnitTest& unit_test) override {
            fprintf(stdout, "\n");
            fprintf(stdout, "TEST SUMMARY:\n");
            fprintf(stdout, "=============\n");
            fprintf(stdout, "Total suites available : %d\n", unit_test.test_suite_to_run_count());
            fprintf(stdout, "Total tests available  : %d\n", unit_test.total_test_count());
            fprintf(stdout, "Total tests disabled   : %d\n", unit_test.disabled_test_count());
            fprintf(stdout, "Total tests passed     : %d\n", unit_test.successful_test_count());
            fprintf(stdout, "Total tests failed     : %d\n", unit_test.failed_test_count());
            fprintf(stdout, "Total tests skipped    : %d\n", unit_test.skipped_test_count());
            fprintf(stdout, "Total time taken       : %d ms", unit_test.elapsed_time());
            fprintf(stdout, "\n");
            fflush(stdout);

            const int disabled_test_count = unit_test.disabled_test_count();
            if (disabled_test_count)
            {
                fprintf(stdout, "\n");
                fprintf(stdout, "%sDISABLED TESTES COUNT: %d%s\n", GetColor(Color::Yellow), disabled_test_count, GetColor(Color::Reset));
            }

            const int failed_test_count = unit_test.failed_test_count();
            if (failed_test_count)
            {
                fprintf(stdout, "\n");
                fprintf(stdout, "\n");
                fprintf(stdout, "%sFAILED TESTES%s\n", GetColor(Color::Red), GetColor(Color::Reset));
                fprintf(stdout, "%s=============%s\n", GetColor(Color::Red), GetColor(Color::Reset));
                int index = 0;
                for (int i = 0; i < unit_test.total_test_suite_count(); ++i) {
                    const TestSuite& test_suite = *unit_test.GetTestSuite(i);
                    if (!test_suite.should_run() || (test_suite.failed_test_count() == 0)) {
                        continue;
                    }
                    std::string suitname(test_suite.name());
                    auto pos = suitname.find("$$");
                    if (pos != std::string::npos) {
                        suitname = suitname.replace(pos, 2, "::");
                    }
                    for (int j = 0; j < test_suite.total_test_count(); ++j) {
                        const TestInfo& test_info = *test_suite.GetTestInfo(j);
                        if (!test_info.should_run() || !test_info.result()->Failed()) {
                            continue;
                        }
                        fprintf(stdout, "%s%d) %s::%s%s", GetColor(Color::Red), ++index, suitname.c_str(), test_info.name(), GetColor(Color::Reset));
                        printf("\n");
                    }
                }
            }
            fflush(stdout);
        }

        // Called before a test starts.
        /*void OnTestStart(const TestInfo& test_info) override {
            fprintf(stdout,
                "*** Test %s.%s starting.\n",
                test_info.test_case_name(),
                test_info.name());
            fflush(stdout);
        }*/

        // Called after a failed assertion or a SUCCEED() invocation.
        void OnTestPartResult(const TestPartResult& test_part_result) override {
            fprintf(stdout, "%s in %s:%d\n%s\n", test_part_result.failed() ? "*** Failure" : "Success", test_part_result.file_name(),
                    test_part_result.line_number(),
                    test_part_result.summary());
            fflush(stdout);
        }
    };  // class CustomPrinter
}  // namespace

int main(int argc, char** argv)
{
    InitGoogleTest(&argc, argv);

    bool Custom_output = true;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--default-output") == 0)
        {
            Custom_output = false;
            break;
        }
    }

    UnitTest& unit_test = *UnitTest::GetInstance();
    if (Custom_output) 
    {
        TestEventListeners& listeners = unit_test.listeners();
        delete listeners.Release(listeners.default_result_printer());
        listeners.Append(new CustomPrinter);
    }

    return RUN_ALL_TESTS();
}	
