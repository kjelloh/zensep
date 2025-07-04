#include "test_atomics.hpp"
#include "test_fixtures.hpp"
#include "../zensep.h"
#include <gtest/gtest.h>
#include <iostream>
#include <immer/box.hpp>

namespace tests::atomics {

    // Basic test to verify the framework is working
    TEST(AtomicTests, DummyAlwaysPass) {
        EXPECT_TRUE(true);
        ASSERT_EQ(1, 1);
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

    // Add more atomic tests here for individual functions
    
    bool run_all() {
        std::cout << "Running atomic tests..." << std::endl;
        
        // Run gtest for this namespace
        ::testing::InitGoogleTest();
        int result = RUN_ALL_TESTS();
        
        return result == 0;
    }
}