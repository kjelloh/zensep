#include "orchestrator.hpp"
#include <string> // std::getline
#include <ranges>
#include <variant>

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

  namespace detail {
    struct NoOp {
      operator bool() const {return false;}
      Window apply(Window const& window) const {
        return window;
      }
    };
  }

  class Formatter {
  public:
    operator bool() const {return false;}
    Window apply(Window const& window) {
      return std::visit(
        [&](auto const& formatter) {
          return formatter.apply(window);
      }
      , m_concrete_formatter);
    }
  private:
    std::variant<detail::NoOp> m_concrete_formatter{};
  };

  Formatter to_formatter(Window const& window) {
    return Formatter{};
  }

  bool is_mutation(Window const& window,auto ix,std::vector<std::string> const& in_cache) {
    return (window.first != in_cache[ix] && window.second != in_cache[ix+1]);
  }

  FormatResult to_formatted(FormatResult const& unformatted, 
                            std::optional<std::pair<size_t, size_t>> line_range) {
    FormatResult result{unformatted};
    result.out.clear();
    
    auto range_to_format = std::make_pair(1, unformatted.in_cache.size());
    if (line_range) {
        range_to_format = *line_range;
    }

    for (int ix=0;ix<result.in_cache.size();ix++) {
      // Is (ix+1),(ix+2) inside range?
      // Note: line_range and range_to_format is 1-index-based while ix is zero-based
      if ((ix+1) >= range_to_format.first and ((ix+1) < range_to_format.second)) {
        Window window{
          result.in_cache[ix] // parent
         ,result.in_cache[ix+1] // child
        };
        while (auto formatter = to_formatter(window)) {
          window = formatter.apply(window);
        }
        // Done formatting
        if (is_mutation(window,ix,result.in_cache)) {
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
        result.out.emplace_back(result.in_cache[ix]);
      }
    }
    return result;
  }

}
