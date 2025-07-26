#include "test_atomics.hpp"
#include "test_fixtures.hpp"
#include "../zensep.h"
#include "../format/Formatter.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <immer/box.hpp>
#include <tao/pegtl.hpp>

namespace tests::atomics {

    // Basic test to verify the framework is working
    TEST(AtomicTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }

    namespace format {

        class FormatterTest : public ::testing::Test {
        protected:
            zensep::Formatter formatter;
        };

        TEST_F(FormatterTest, FunctionWithArguments) {
            std::string input = R"(void foo(int a, double b, std::string c);)";
            std::string expected = R"(void foo(
    int a
    , double b
    , std::string c);
)";
            EXPECT_EQ(formatter.format(input), expected);
        }

        TEST_F(FormatterTest, FunctionWithBlockStatements) {
            std::string input = R"(void bar() { int x = 0; x++; std::cout << x; })";
            std::string expected = R"(void bar(
){
    int x
    = 0;
    x++;
    std::cout<<x;
})";
            EXPECT_EQ(formatter.format(input), expected);
        }

        TEST_F(FormatterTest, TernaryExpression) {
            std::string input = R"(int value = (x > 0) ? x * 2 : y / 2;)";
            std::string expected = R"(int value
= (
    x
    > 0)
? x
* 2
: y
/ 2;
)";
            EXPECT_EQ(formatter.format(input), expected);
        }

        TEST_F(FormatterTest, NestedScopesAndExpressions) {
            std::string input = R"(auto result = a + (b * (c + d / (e - f))) - g;)";
            std::string expected = R"(auto result
= a
+ (
    b
    * (
        c
        + d
        / (
            e
            - f        )    ))
- g;
)";
            EXPECT_EQ(formatter.format(input), expected);
        }

        TEST_F(FormatterTest, ConstructorWithMultipleParameters) {
            std::string input = R"(    ProjectState(StateImpl::UX ux, PersistentFile<Environment> m_persistent_environment_file,Environment 
  m_environment);)";
            std::string expected = R"(    ProjectState(
         StateImpl::UX ux
        ,PersistentFile<Environment> m_persistent_environment_file
        ,Environment m_environment);
)";
            EXPECT_EQ(formatter.format(input), expected);
        }

    }

    namespace immer {
        // Test immer::box compilation and linking
        TEST(AtomicTests, ImmerBoxBasicUsage) {
            // Create an immer::box with an integer
            ::immer::box<int> box1{42};
            
            // Verify the value
            EXPECT_EQ(*box1, 42);
            
            // Create another box with a different value
            ::immer::box<int> box2{100};
            EXPECT_EQ(*box2, 100);
            
            // Test copy construction
            ::immer::box<int> box3 = box1;
            EXPECT_EQ(*box3, 42);
            
            // Test with string
            ::immer::box<std::string> string_box{"hello immer"};
            EXPECT_EQ(*string_box, "hello immer");
            
            // This test ensures immer compiles and links correctly
            // Not testing immer functionality, just integration
        }
    }

    namespace pegtl {
        // Test PEGTL compilation and linking
        TEST(AtomicTests, PegtlBasicUsage) {
            // Simple grammar: match one or more digits
            namespace pegtl = tao::pegtl;
            
            struct digit_rule : pegtl::plus<pegtl::digit> {};
            struct grammar : pegtl::seq<digit_rule, pegtl::eof> {};
            
            // Test parsing a simple number
            std::string input = "12345";
            pegtl::memory_input in(input, "test");
            
            // This should parse successfully
            bool parse_result = pegtl::parse<grammar>(in);
            EXPECT_TRUE(parse_result);
            
            // Test with invalid input (use try/catch for safety)
            std::string invalid_input = "abc";
            pegtl::memory_input invalid_in(invalid_input, "test");
            
            // This should fail to parse
            bool invalid_parse_result = false;
            try {
                invalid_parse_result = pegtl::parse<grammar>(invalid_in);
            } catch (...) {
                // Expected to fail
                invalid_parse_result = false;
            }
            EXPECT_FALSE(invalid_parse_result);
            
            // This test ensures PEGTL compiles and links correctly
            // Not testing PEGTL functionality extensively, just integration
        }
    }

    // Add more atomic tests here for individual functions
    
    bool run_all() {
        std::cout << "Running atomic tests..." << std::endl;
        
        // Filter to run only atomic tests (AtomicTests and FormatterTest suites)
        ::testing::GTEST_FLAG(filter) = "AtomicTests*:FormatterTest*";
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}