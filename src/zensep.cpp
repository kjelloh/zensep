#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <utility>
#include "zensep.h"

// C++ implementation namespace
namespace zensep {
  // Add the missing format_file function to namespace zensep
  void format_file(const std::filesystem::path& input_file,
                   std::ostream& output_stream,
                   const std::string& lines_range,
                   bool dry_run) {
    // Read input file
    std::ifstream input(input_file);
    if (!input.is_open()) { throw std::runtime_error("Cannot open file: " + input_file.string()); }

    if (dry_run) {
      output_stream << "DRY RUN: Would format " << input_file.filename().string();
      if (!lines_range.empty()) { output_stream << " (lines " << lines_range << ")"; }
      output_stream << "\n";
      output_stream << "No changes would be made (no-op formatter)\n";
      return;
    }

    // Read and output file line by line (no-op for now)
    std::string line;
    while (std::getline(input, line)) {
      // TODO: Add actual formatting logic here
      // For now, just pass through unchanged
      output_stream << line << "\n";
    }
  }

  void print_build_info() {
    #ifdef NDEBUG
    std::cout << "zensep/0.0: Hello World Release!\n";
    #else
        std::cout << "zensep/0.0: Hello World Debug!\n";
    #endif

    // ARCHITECTURES
    #ifdef _M_X64
        std::cout << "  zensep/0.0: _M_X64 defined\n";
    #endif

    #ifdef _M_IX86
        std::cout << "  zensep/0.0: _M_IX86 defined\n";
    #endif

    #ifdef _M_ARM64
        std::cout << "  zensep/0.0: _M_ARM64 defined\n";
    #endif

    #if __i386__
        std::cout << "  zensep/0.0: __i386__ defined\n";
    #endif

    #if __x86_64__
        std::cout << "  zensep/0.0: __x86_64__ defined\n";
    #endif

    #if __aarch64__
    std::cout << "  zensep/0.0: __aarch64__ defined\n";
    #endif

    // Libstdc++
    #if defined _GLIBCXX_USE_CXX11_ABI
        std::cout << "  zensep/0.0: _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
    #endif

    // MSVC runtime
    #if defined(_DEBUG)
    #if defined(_MT) && defined(_DLL)
            std::cout << "  zensep/0.0: MSVC runtime: MultiThreadedDebugDLL\n";
    #elif defined(_MT)
            std::cout << "  zensep/0.0: MSVC runtime: MultiThreadedDebug\n";
    #endif
    #else
    #if defined(_MT) && defined(_DLL)
            std::cout << "  zensep/0.0: MSVC runtime: MultiThreadedDLL\n";
    #elif defined(_MT)
            std::cout << "  zensep/0.0: MSVC runtime: MultiThreaded\n";
    #endif
    #endif

    // COMPILER VERSIONS
    #if _MSC_VER
        std::cout << "  zensep/0.0: _MSC_VER" << _MSC_VER<< "\n";
    #endif

    #if _MSVC_LANG
        std::cout << "  zensep/0.0: _MSVC_LANG" << _MSVC_LANG<< "\n";
    #endif

    #if __cplusplus
    std::cout << "  zensep/0.0: __cplusplus" << __cplusplus << "\n";
    #endif

    #if __INTEL_COMPILER
        std::cout << "  zensep/0.0: __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
    #endif

    #if __GNUC__
    std::cout << "  zensep/0.0: __GNUC__" << __GNUC__ << "\n";
    #endif

    #if __GNUC_MINOR__
    std::cout << "  zensep/0.0: __GNUC_MINOR__" << __GNUC_MINOR__ << "\n";
    #endif

    #if __clang_major__
    std::cout << "  zensep/0.0: __clang_major__" << __clang_major__ << "\n";
    #endif

    #if __clang_minor__
        std::cout << "  zensep/0.0: __clang_minor__" << __clang_minor__<< "\n";
    #endif

    #if __apple_build_version__
    std::cout << "  zensep/0.0: __apple_build_version__" << __apple_build_version__ << "\n";
    #endif

    // SUBSYSTEMS

    #if __MSYS__
        std::cout << "  zensep/0.0: __MSYS__" << __MSYS__<< "\n";
    #endif

    #if __MINGW32__
        std::cout << "  zensep/0.0: __MINGW32__" << __MINGW32__<< "\n";
    #endif

    #if __MINGW64__
        std::cout << "  zensep/0.0: __MINGW64__" << __MINGW64__<< "\n";
    #endif

    #if __CYGWIN__
        std::cout << "  zensep/0.0: __CYGWIN__" << __CYGWIN__<< "\n";
    #endif
  }
} // zensep

// C API implementations that call the C++ versions
void print_build_info(void) { zensep::print_build_info(); }

int format_file(const char* input_filename,
                const char* output_filename,
                const char* lines_range,
                int dry_run) {
  try {
    std::filesystem::path input_path(input_filename);
    std::string range_str = lines_range ? lines_range : "";
    bool is_dry_run = (dry_run != 0);

    if (output_filename) {
      // Output to file (in-place)
      std::ofstream output_file(output_filename);
      if (!output_file.is_open()) {
        return 2; // Error opening output file
      }
      zensep::format_file(input_path, output_file, range_str, is_dry_run);
    }
    else {
      // Output to stdout
      zensep::format_file(input_path, std::cout, range_str, is_dry_run);
    }

    return 0; // Success
  }
  catch (const std::filesystem::filesystem_error&) {
    return 2; // File system error (file not found, permission denied, etc.)
  } catch (const std::invalid_argument&) {
    return 3; // Invalid line range format
  } catch (...) {
    return 1; // General error
  }
}
