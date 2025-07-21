#include "Parsers.hpp"
#include <cctype>

namespace zensep::parse {

    ParseResult StatementParser::operator()(std::string_view input) const {
        if (input.empty()) {
            return ParseResult::failure("No input to parse");
        }
        
        // Skip leading whitespace to find first non-whitespace
        size_t start = 0;
        while (start < input.size() && std::isspace(input[start])) {
            ++start;
        }
        
        if (start >= input.size()) {
            return ParseResult::failure("Only whitespace found");
        }
        
        // Find semicolon from the first non-whitespace position
        size_t semicolon_pos = input.find(';', start);
        if (semicolon_pos == std::string_view::npos) {
            return ParseResult::failure("No semicolon found");
        }
        
        // Include the semicolon in the match
        size_t end = semicolon_pos + 1;
        return ParseResult::success(input.substr(0, end), end);
    }

    ParseResult MatchAllParser::operator()(std::string_view input) const {
        if (input.empty()) {
            return ParseResult::failure("No input to match");
        }
        return ParseResult::success(input, input.size());
    }

}