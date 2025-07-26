#include "Formatter.hpp"
#include <string> // std::getline
#include <variant>
#include <queue>
#include <sstream>
#include <stack>
#include <format>
#include <limits>

namespace zensep {

  Formatter::Formatter(Options const& options) : m_options{options} {}

  Formatter::FormatResult NoOpFormatter::format(const std::string& source) {
    std::istringstream in{source};
    std::ostringstream output;
    
    // Handle line range properly - default to entire file if no range specified
    auto first_line_ix = (m_options.line_range) ? m_options.line_range->first - 1 : 0;
    auto last_line_ix = (m_options.line_range) ? m_options.line_range->second - 1 : std::numeric_limits<size_t>::max();
    
    std::string entry;
    
    // Output lines before the target range
    for (decltype(first_line_ix) i = 0; i < first_line_ix; ++i) {
      if (std::getline(in, entry)) {
        output << entry << "\n";
      }
    }
    
    // Collect target lines for formatting
    std::string target{};
    for (auto i = first_line_ix; i <= last_line_ix && std::getline(in, entry); ++i) {
      target += entry;
      target += "\n";
    }

    output << "// Zensep format BEGIN\n";
    output << target;
    output << "// Zensep format END\n";

    // Output remaining lines after the target range
    while (std::getline(in, entry)) {
      output << entry << "\n";
    }
    
    return output.str();
  }

}
