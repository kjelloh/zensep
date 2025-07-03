#pragma once

#include <string>
#include <format>

// Standalone fallback versions (used when not built with CMake)
#ifndef ZENSEP_VERSION
#define ZENSEP_VERSION "0.0-standalone"
#endif

#ifndef ZENSEP_GIT_VERSION
#define ZENSEP_GIT_VERSION "unknown"
#endif

namespace zensep {
    // Get the main version
    inline std::string version() {
        return ZENSEP_VERSION;
    }
    
    // Get the git version
    inline std::string git_version() {
        return ZENSEP_GIT_VERSION;
    }
    
    // Get combined version string
    inline std::string full_version() {
        return std::format("{} (git: {})", version(), git_version());
    }
}