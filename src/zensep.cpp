#include "zensep.h"
#include "format/orchestrator.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm> // std::ranges::copy,
#include <iterator> // ostream_iterator,
#include <expected>
#include <stdexcept>

namespace zensep {

  namespace detail {

    using LineRange = std::pair<size_t, size_t>;
    
    std::expected<LineRange, std::string> parse_line_range(std::string_view range_str) {
        auto colon_pos = range_str.find(':');
        if (colon_pos == std::string_view::npos) {
            return std::unexpected("Invalid format: expected 'start:end' (e.g., '10:20')");
        }
        
        try {
            std::string start_str(range_str.substr(0, colon_pos));
            std::string end_str(range_str.substr(colon_pos + 1));
            
            long start_line = std::stol(start_str);
            long end_line = std::stol(end_str);
            
            if (start_line <= 0) {
                return std::unexpected("Start line must be greater than 0");
            }
            if (end_line <= 0) {
                return std::unexpected("End line must be greater than 0");
            }
            if (start_line > end_line) {
                return std::unexpected("Start line cannot be greater than end line");
            }
            
            return LineRange{static_cast<size_t>(start_line), static_cast<size_t>(end_line)};
        }
        catch (const std::invalid_argument&) {
            return std::unexpected("Invalid number format in line range");
        }
        catch (const std::out_of_range&) {
            return std::unexpected("Line numbers too large");
        }
    }

    void print_lines(std::ostream& out, const std::ranges::range auto& lines) {
      std::ranges::copy(lines,
                        std::ostream_iterator<std::string_view>(out, "\n"));
    }

  }
  // Add the missing format_file function to namespace zensep
  void format_file(const std::filesystem::path& input_file,
                   std::ostream& output_stream,
                   const std::string& lines_range,
                   bool dry_run) {
    // Read input file
    std::ifstream input(input_file);
    if (!input.is_open()) { throw std::runtime_error("Cannot open file: " + input_file.string()); }

    // Parse line range if provided
    std::optional<detail::LineRange> parsed_range;
    if (!lines_range.empty()) {
      auto parse_result = detail::parse_line_range(lines_range);
      if (!parse_result) {
        std::cerr << "Line range error: " << parse_result.error() << std::endl;
        throw std::invalid_argument("Line range error: " + parse_result.error());
      }
      parsed_range = parse_result.value();
    }

    auto unformatted = format::to_unformatted(input);

    if (dry_run) {
      output_stream << "DRY RUN: Would format " << input_file.filename().string();
      if (!lines_range.empty()) { output_stream << " (lines " << lines_range << ")"; }
      output_stream << "\n";
      output_stream << "No changes would be made (no-op formatter)\n";
      return;
    }

    auto formatted = format::to_formatted(unformatted, parsed_range);
    detail::print_lines(output_stream,formatted.out);
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
