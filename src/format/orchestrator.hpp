#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace zensep {

  class Formatter {
  public:
    std::string format(const std::string& source);
  };

}
namespace zensep::format {

  struct FormatResult {
    std::vector<std::string> in_cache{}; // input cache
    std::vector<std::string> out_cache{}; // mutated cache
    std::vector<std::string_view> out; // Refs to in or mutated
  };

  FormatResult to_unformatted(std::ifstream& input);

  FormatResult to_formatted(
     FormatResult const& unformatted
    ,std::optional<std::pair<size_t, size_t>> line_range = std::nullopt);

}
