#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include "../zensep.h"
#include <gtest/gtest.h>
#include <iostream>

namespace tests::integrations {

    // Basic integration test
    TEST(IntegrationTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
    }

    // Add more integration tests here for end-to-end scenarios
    
    bool run_all() {
        std::cout << "Running integration tests..." << std::endl;
        
        // Run gtest for this namespace
        ::testing::InitGoogleTest();
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}