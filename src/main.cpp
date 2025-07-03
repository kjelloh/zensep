#include "zensep.h"
#include "version.h"
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
    std::string lines_range;
    
    // Positional arguments for files (standard formatter behavior)
    app.add_option("files", files, "Input files to format");
    
    // Flags
    app.add_flag("-i,--in-place", in_place, "Modify files in place");
    app.add_option("--lines", lines_range, "Format specific line range (start:end)");
    app.add_flag("-n,--dry-run", dry_run, "Show changes without modifying files");
    app.add_flag("--build-info", show_build_info, "Show detailed build and compiler information");
    
    CLI11_PARSE(app, argc, argv);
    
    if (show_build_info) {
        print_build_info();
        return 0;
    }
    
    if (files.empty()) {
        std::cout << "No files specified. Use --help for usage information.\n";
        return 1;
    }
    
    // TODO: Implement actual formatting functionality
    std::cout << "Zensep formatter - functionality coming soon!\n";
    std::cout << "Would format " << files.size() << " file(s)";
    if (in_place) std::cout << " in-place";
    if (dry_run) std::cout << " (dry-run)";
    if (!lines_range.empty()) std::cout << " lines " << lines_range;
    std::cout << "\n";
    
    return 0;
}
