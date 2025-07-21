#pragma once

#include "Parser.hpp"

namespace zensep::parse {

    class MatchAllParser {
    public:
        ParseResult operator()(std::string_view input) const;
    };

}