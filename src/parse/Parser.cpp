#include "Parser.hpp"
#include "Parsers.hpp"

namespace zensep::parse {

    std::optional<ParsePassResult> parse_pass(
        std::string_view input,
        const std::vector<Parser>& parsers
    ) {
        for (size_t pos = 0; pos < input.size(); ++pos) {
            auto remaining = input.substr(pos);
            
            for (size_t i = 0; i < parsers.size(); ++i) {
                auto result = std::visit([remaining](const auto& parser) {
                    return parser(remaining);
                }, parsers[i]);
                
                if (result.matched) {
                    return ParsePassResult{
                        input.substr(0, pos),
                        result.content,
                        remaining.substr(result.consumed_chars),
                        i
                    };
                }
            }
        }
        return std::nullopt;
    }

}