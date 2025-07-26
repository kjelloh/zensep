#include "test_runner.hpp"
#include "test_atomics.hpp"
#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace tests {
    bool run_all() {
        std::cout << "Running all tests..." << std::endl;

        // Create and register global test environment
        auto* testEnv = new fixtures::GlobalCLIFixture();
        ::testing::AddGlobalTestEnvironment(testEnv);

        // Initialize GoogleTest once at the top level
        ::testing::InitGoogleTest();
        
        bool atomics_pass = atomics::run_all();
        bool integrations_pass = integrations::run_all();
        
        bool all_pass = atomics_pass && integrations_pass;
        
        std::cout << "All tests " << (all_pass ? "PASSED" : "FAILED") << std::endl;
        return all_pass;
    }
}