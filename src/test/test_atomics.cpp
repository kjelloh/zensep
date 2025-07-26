#include "test_atomics.hpp"
#include "test_fixtures.hpp"
#include "../format/Formatter.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <tao/pegtl.hpp>

namespace tests::atomics {

    // Basic test to verify the framework is working
    TEST(AtomicTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }
    
    bool run_all() {
        std::cout << "Running atomic tests..." << std::endl;
        
        // Filter to run only atomic tests (AtomicTests and FormatterTest suites)
        ::testing::GTEST_FLAG(filter) = "AtomicTests*:FormatterTest*";
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}