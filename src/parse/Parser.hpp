#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>
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
        
        static ParseResult failure(const std::string& error = "") {
            return {false, {}, 0, error.empty() ? std::nullopt : std::make_optional(error)};
        }
    };

    struct ParsePassResult {
        std::string_view prefix;
        std::string_view matched;
        std::string_view postfix;
        size_t parser_index;
    };

    class MatchAllParser;

    using Parser = std::variant<
        MatchAllParser
    >;

    std::optional<ParsePassResult> parse_pass(
        std::string_view input,
        const std::vector<Parser>& parsers
    );

}