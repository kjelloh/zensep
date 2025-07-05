#include "orchestrator.hpp"
#include <string> // std::getline
#include <variant>
#include <queue>

namespace zensep::format {
  FormatResult to_unformatted(std::ifstream& input) {
    FormatResult result;
    for (std::string line; std::getline(input, line);) {
      result.in_cache.push_back(std::move(line));
    }
    std::ranges::copy(result.in_cache, std::back_inserter(result.out));
    return result;
  }


  namespace detail {
    using Baked = std::string;
    struct NoOp {
      Baked visit(std::queue<std::string>& current) const {
        auto result = current.front();
        current.pop();
        return result;
      }
    };
  }
  using Baked = detail::Baked;

  class Formatter {
  public:
    Baked visit(std::queue<std::string>& current) const {
      return std::visit([&](auto const& concrete_formatter) {
        return concrete_formatter.visit(current);
      },m_concrete_formatter);
    }
  private:
    std::variant<detail::NoOp> m_concrete_formatter{};
  };

  FormatResult to_formatted(FormatResult const& unformatted, 
                            std::optional<std::pair<size_t, size_t>> line_range) {
    FormatResult result{unformatted};

    auto range_to_format = std::make_pair(1, unformatted.in_cache.size());
    if (line_range) {
        range_to_format = *line_range;
    }

    struct Unvisited {
      int ix;
      std::string_view in_cache_line;
    };
    // Consume the input using a queue of views into in_cache.
    std::queue<Unvisited> unvisited{};
    for (int ix=0;ix<result.in_cache.size();++ix) unvisited.push({ix, result.in_cache[ix]});

    if (true) {
      result.out.clear();
      // while (unvisited)
      //  feed: (unvisited,current)  -> (unvisited,current)
      //  update: (current,formatter_stack) -> formatter_stack
      //  format: (current,formatter_stack) -> (current,baked)
      //  emit: (baked,result) -> result

      std::queue<std::string> current{};
      auto feed = [](std::queue<Unvisited>& unvisited,std::queue<std::string>& current) {
        while (!unvisited.empty() and current.size() < 2) {
          current.push(std::string{unvisited.front().in_cache_line});
          unvisited.pop();
        }
      };

      std::stack<Formatter> formatter_stack{};
      auto update = [](std::queue<std::string>& current,std::stack<Formatter>& formatter_stack) {
        if (formatter_stack.empty()) {
          formatter_stack.push(Formatter{});
        }
      };

      auto format = [](std::queue<std::string>& current,std::stack<Formatter>& formatter_stack) {
        return formatter_stack.top().visit(current);
      };

      auto emit = [](Baked const& baked,FormatResult& result) {
        result.out_cache.push_back(baked);
        result.out.push_back(result.out_cache.back());
      };

      while (unvisited.size() > 0) {
        feed(unvisited,current);
        update(current,formatter_stack);
        auto baked = format(current,formatter_stack);
        emit(baked,result);
      }

    }

    return result;
  }

}
