#include "Parsers.hpp"

namespace zensep::parse {

    ParseResult MatchAllParser::operator()(std::string_view input) const {
        if (input.empty()) {
            return ParseResult::failure("No input to match");
        }
        return ParseResult::success(input, input.size());
    }

}