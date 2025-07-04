#include "zensep.h"
#include "version.h"
#include "test/test_runner.hpp"
#include <CLI/CLI.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    CLI::App app{"Zensep - A separator-first C++ code formatter", "zensep"};
    app.set_version_flag("-v,--version", zensep::full_version());
    
    // Core formatter arguments
    std::vector<std::string> files;
    bool in_place = false;
    bool dry_run = false;
    bool show_build_info = false;
    bool run_tests = false;
    std::string lines_range;

    // Positional arguments for files (standard formatter behavior)
    app.add_option("files", files, "Input files to format");
    
    // Flags
    app.add_flag("-i,--in-place", in_place, "Modify files in place");
    app.add_option("--lines", lines_range, "Format specific line range (start:end)");
    app.add_flag("-n,--dry-run", dry_run, "Show changes without modifying files");
    app.add_flag("--build-info", show_build_info, "Show detailed build and compiler information");
    app.add_flag("--test", run_tests, "Run all tests and exit");

    CLI11_PARSE(app, argc, argv);
    
    if (show_build_info) {
        print_build_info();
        return 0;
    }
    
    if (run_tests) {
        bool tests_passed = tests::run_all();
        return tests_passed ? 0 : 1;
    }
    
    if (files.empty()) {
        std::cout << "No files specified. Use --help for usage information.\n";
        return 1;
    }
    
    // Process each file - break on first error
    int result = 0;
    for (const auto& file : files) {
        const char* output_file = in_place ? file.c_str() : nullptr;
        const char* range = lines_range.empty() ? nullptr : lines_range.c_str();
        
        result = format_file(file.c_str(), output_file, range, dry_run ? 1 : 0);
        
        if (result != 0) {
            std::cerr << "Error formatting file: " << file << " (exit code: " << result << ")\n";
            break; // Break on error, let fall-through handle return
        }
    }
    
    return result;
}
