#include "orchestrator.hpp"
#include <string> // std::getline
#include <ranges>

namespace zensep::format {
  FormatResult to_unformatted(std::ifstream& input) {
    FormatResult result;
    for (std::string line; std::getline(input, line);) {
      result.in_cache.push_back(std::move(line));
    }
    std::ranges::copy(result.in_cache, std::back_inserter(result.out));
    return result;
  }

  using Window = std::pair<std::string,std::string>;

  class Formatter {
  public:
    operator bool() const {return false;}
    Window apply(Window const& window) {
      return window; // No op for now
    }
  private:
  };
  Formatter to_formatter(Window const& window) {
    return Formatter{};
  }

  FormatResult to_formatted(FormatResult const& unformatted, 
                            std::optional<std::pair<size_t, size_t>> line_range) {
    FormatResult result{unformatted};
    result.out_cache.clear();
    result.out.clear();
    
    auto range_to_format = std::make_pair(1, unformatted.in_cache.size());
    if (line_range) {
        range_to_format = *line_range;
    }

    for (int ix=0;ix<result.in_cache.size();ix++) {

      // Is (ix+1),(ix+2) inside range?
      if ((ix+1) >= range_to_format.first and ((ix+1) < range_to_format.second)) {
        Window window{
          result.in_cache[ix] // current
         ,result.in_cache[ix+1] // child
        };
        if (auto formatter = to_formatter(window)) {
          while (formatter) {
            window = formatter.apply(window);
          }
          // Done formatting
          if (window.first.size()>0) {
            result.out_cache.push_back(window.first);
            result.out_cache.push_back(result.out_cache.back());
          }
          if (window.second.size()>0) {
            result.out_cache.push_back(window.second);
            result.out_cache.push_back(result.out_cache.back());
          }
        }
        else {
          result.out.emplace_back(result.in_cache[ix]); // as is
        }
      }
      else {
        result.out.emplace_back(result.in_cache[ix]); // as is
      }
    }

    int line_no{0};
    for (auto const& line : unformatted.in_cache) {
        ++line_no; // 1,2,3,...

        // Work on two lines at a time
        std::pair<std::string,std::string> lines_pair{};
        if (line_no >= range_to_format.first && line_no <= range_to_format.second) {

            result.out.emplace_back(line);
        } else {
            // Outside range, use original
            result.out.emplace_back(line);
        }
    }
    
    return result;
  }

}
