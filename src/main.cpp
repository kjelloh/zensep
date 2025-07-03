#include "zensep.h"
#include "version.h"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char** argv) {
    CLI::App app{"Zensep - A separator-first C++ code formatter", "zensep"};
    app.set_version_flag("-v,--version", zensep::full_version());
    
    bool show_build_info = false;
    app.add_flag("--build-info", show_build_info, "Show detailed build and compiler information");
    
    CLI11_PARSE(app, argc, argv);
    
    if (show_build_info) {
        print_build_info();
        return 0;
    }
    
    // TODO: Implement actual formatting functionality
    std::cout << "Zensep formatter - functionality coming soon!\n";
    
    return 0;
}
