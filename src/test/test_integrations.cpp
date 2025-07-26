#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include "../zensep.h"
#include <gtest/gtest.h>
#include <iostream>

namespace tests::integrations {

    // Create an alias for the fixture to make the name usable in TEST_F macro (don't use namespace path)
    using CLITestFixture = tests::fixtures::CLITestFixture;

    // Basic integration test
    TEST(IntegrationTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }

    TEST_F(CLITestFixture, DummyAlwaysPass) {
        // Create test input file
        std::string input = R"(void foo(int a, double b, std::string c);)";
        writeFile("test.cpp", input);
        
        // Run zensep and capture output
        std::string output = runZensepAndGetOutput("test.cpp");
                
        EXPECT_EQ(output,output);
    }
    
    bool run_all() {
        std::cout << "Running integration tests..." << std::endl;
        
        // Filter to run integration tests (IntegrationTests suite and CLITestFixture suite)
        ::testing::GTEST_FLAG(filter) = "IntegrationTests*:CLITestFixture*";
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}