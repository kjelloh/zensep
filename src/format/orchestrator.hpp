#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace zensep {

  class Formatter {
  public:
    struct Options {
      std::optional<std::pair<size_t, size_t>> line_range;
    };
    using Result = std::string;
    Formatter::Result format(const std::string& source, const Formatter::Options& options = {});
  };

}
