#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace zensep::parse {

  struct ParseResult {
    bool matched;
    std::string_view content;
    size_t consumed_chars;
    std::optional<std::string> error_msg;

    static ParseResult success(std::string_view content, size_t consumed) {
      return {true, content, consumed, std::nullopt};
    }

    static ParseResult failure(const std::string &error = "") {
      return {
          false, {}, 0, error.empty() ? std::nullopt : std::make_optional(error)};
    }
  };

  class StatementParser {
  public:
    ParseResult operator()(std::string_view input) const;
  };

  class MatchAllParser {
  public:
    ParseResult operator()(std::string_view input) const;
  };

} // namespace zensep::parse