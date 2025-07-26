#pragma once
#include "../format/Formatter.hpp"
#include <gtest/gtest.h>
#include <filesystem>

namespace tests::fixtures {
    // Shared test fixtures and utilities

    // GlobalCLIFixture class definition
    class GlobalCLIFixture : public ::testing::Environment {
    public:
        GlobalCLIFixture() { instance = this; }
        
        void SetUp() override;
        void TearDown() override;
        
        // Getters for shared resources
        const std::string& getRootTestDir() const { return rootTestDir; }
        const std::string& getZensepExecutable() const { return zensepExecutable; }
        
        // Static access method
        static GlobalCLIFixture* getInstance() { return instance; }

    private:
        std::string rootTestDir;
        std::string zensepExecutable;
        static GlobalCLIFixture* instance;

    };
    
    class CLITestFixture : public ::testing::Test {
    protected:
        std::filesystem::path testDir;
        
    public:
        CLITestFixture() = default;
        
        void SetUp() override;
        void TearDown() override;
        bool runZensepOn(const std::string& cppFile);
        std::string runZensepAndGetOutput(const std::string& cppFile);
        std::string readFile(const std::string& path);
        bool runZensepWithArgs(const std::vector<std::string>& args);
        void writeFile(const std::string& path, const std::string& content);
        
    private:
        std::string getZensepExecutable();
        std::filesystem::path getFullPath(const std::string& filename);
    };

}