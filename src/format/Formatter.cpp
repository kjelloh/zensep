#include "Formatter.hpp"
#include <string> // std::getline
#include <variant>
#include <queue>
#include <sstream>
#include <stack>

namespace zensep {

  Formatter::Formatter(Options const& options) : m_options{options} {}

  Formatter::FormatResult NoOpFormatter::format(const std::string& source) {
    std::ostringstream output;
    output << std::format("// Zensep format BEGIN");
    output << source;
    output << std::format("// Zensep format END");
    return output.str();
  }

}
