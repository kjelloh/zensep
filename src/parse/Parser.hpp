#pragma once

#include "Parsers.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace zensep::parse {

    struct ParsePassResult {
        std::string_view prefix;
        std::string_view matched;
        std::string_view postfix;
        size_t parser_index;
    };
    
    using Parser = std::variant<
        StatementParser,
        MatchAllParser
    >;

    std::optional<ParsePassResult> parse_pass(
        std::string_view input,
        const std::vector<Parser>& parsers
    );

}