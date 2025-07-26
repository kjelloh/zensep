#include "test_fixtures.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

namespace tests::fixtures {

    // Static instance for GlobalCLIFixture (defaults to nullptr)
    GlobalCLIFixture* GlobalCLIFixture::instance = nullptr;

    // GlobalCLIFixture implementation
    void GlobalCLIFixture::SetUp() {
        rootTestDir = std::filesystem::temp_directory_path() / "zensep_test_env";
        std::filesystem::create_directories(rootTestDir);
        
        // Assume zensep executable is in current directory (we are run as 'zensep --test')
        zensepExecutable = std::filesystem::current_path() / "zensep";
        
        std::cout << "GlobalCLIFixture: Set up root test dir " << rootTestDir << std::endl;
        std::cout << "GlobalCLIFixture: Using zensep at " << zensepExecutable << std::endl;
    }
    
    void GlobalCLIFixture::TearDown() {
        std::cout << "GlobalCLIFixture: Tore down root test dir " << rootTestDir << std::endl;
        // Clean up root test directory
        if (std::filesystem::exists(rootTestDir)) {
            std::filesystem::remove_all(rootTestDir);
        }
    }
    
    // CLITestFixture implementation
    void CLITestFixture::SetUp() {
        // Use shared root directory from GlobalCLIFixture
        auto* env = GlobalCLIFixture::getInstance();
        testDir = std::filesystem::path(env->getRootTestDir()) / ("test_" + std::to_string(std::rand()));
        std::filesystem::create_directories(testDir);
    }
    
    void CLITestFixture::TearDown() {
        // Clean up test directory
        std::filesystem::remove_all(testDir);
    }
    
    std::string CLITestFixture::getZensepExecutable() {
        // Use shared zensep path from GlobalCLIFixture
        return GlobalCLIFixture::getInstance()->getZensepExecutable();
    }
    
    std::filesystem::path CLITestFixture::getFullPath(const std::string& filename) {
        return testDir / filename;
    }
    
    bool CLITestFixture::runZensepOn(const std::string& cppFile) {
        // Build command with full path to input file
        std::string command = getZensepExecutable() + " " + getFullPath(cppFile).string();
        
        // Execute the command
        int result = std::system(command.c_str());
        
        // Return true if command succeeded (exit code 0)
        return result == 0;
    }
    
    std::string CLITestFixture::runZensepAndGetOutput(const std::string& cppFile) {
        // Build command with output redirection and full path to input file
        std::string command = getZensepExecutable() + " " + getFullPath(cppFile).string() + " 2>&1";
        
        // Execute and capture output
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("Failed to run zensep command");
        }
        
        std::string output;
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            output += buffer;
        }
        
        pclose(pipe);
        return output;
    }
    
    std::string CLITestFixture::readFile(const std::string& path) {
        std::ifstream file(getFullPath(path));
        if (!file.is_open()) {
            throw std::runtime_error("Failed to read file: " + path);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    bool CLITestFixture::runZensepWithArgs(const std::vector<std::string>& args) {
        // Build command with arguments (use full paths for file arguments)
        std::string command = getZensepExecutable();
        for (const auto& arg : args) {
            command += " ";
            // If argument looks like a filename, use full path
            if (arg.find(".cpp") != std::string::npos || arg.find(".h") != std::string::npos) {
                command += getFullPath(arg).string();
            } else {
                command += arg;
            }
        }
        
        // Execute the command
        int result = std::system(command.c_str());
        
        // Return true if command succeeded (exit code 0)
        return result == 0;
    }
    
    void CLITestFixture::writeFile(const std::string& path, const std::string& content) {
        std::ofstream file(getFullPath(path));
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create test file: " + path);
        }
        file << content;
        file.close();
    }

}