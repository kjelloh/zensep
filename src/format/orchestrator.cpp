#include "orchestrator.hpp"
#include <string> // std::getline

namespace zensep::format {
  FormatResult to_unformatted(std::ifstream& input) {
    FormatResult result;
    for (std::string line; std::getline(input, line);) {
      result.in.push_back(std::move(line));
    }
    std::ranges::copy(result.in, std::back_inserter(result.out));
    return result;
  }

  FormatResult to_formatted(FormatResult const& unformatted, 
                            std::optional<std::pair<size_t, size_t>> line_range) {
    // TODO: Use line_range when implementing actual formatting
    return unformatted; // No op for now
  }

}
