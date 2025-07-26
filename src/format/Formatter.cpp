#include "Formatter.hpp"
#include <string> // std::getline
#include <variant>
#include <queue>
#include <sstream>
#include <stack>

namespace zensep {

  namespace {
    bool isSeparator(char c) {
      return c == ',' || c == '+' || c == '-' || c == '*' || c == '/' ||
             c == '%' || c == '=' || c == '?' || c == ':' ||
             c == '&' || c == '|' || c == '<' || c == '>' || c == '!';
    }

    bool isScopeOpen(char c) {
      return c == '{' || c == '(' || c == '[';
    }

    bool isScopeClose(char c) {
      return c == '}' || c == ')' || c == ']';
    }

    std::string indent(int level) {
      return std::string(level * 4, ' ');
    }
  }

  Formatter::Result Formatter::format(const std::string& source, const Formatter::Options& options) {
    std::ostringstream output;
    int indentLevel = 0;
    bool newLine = true;
    bool needSpace = false;

    for (size_t i = 0; i < source.length(); ++i) {
      char c = source[i];

      if (std::isspace(c)) {
        // Mark that we need a space if we're between non-separator characters
        if (!newLine && i > 0 && i < source.length() - 1) {
          char prev = source[i-1];
          char next = source[i+1];
          if (std::isalnum(prev) && std::isalnum(next)) {
            needSpace = true;
          }
        }
        continue;
      }

      // Handle multi-character operators
      if (c == ':' && i + 1 < source.length() && source[i+1] == ':') {
        if (newLine) output << indent(indentLevel);
        else if (needSpace) output << " ";
        output << "::";
        needSpace = false;
        newLine = false;
        ++i; // skip next ':'
        continue;
      }

      if (c == '+' && i + 1 < source.length() && source[i+1] == '+') {
        if (newLine) output << indent(indentLevel);
        else if (needSpace) output << " ";
        output << "++";
        needSpace = false;
        newLine = false;
        ++i; // skip next '+'
        continue;
      }

      if (c == '<' && i + 1 < source.length() && source[i+1] == '<') {
        if (newLine) output << indent(indentLevel);
        else if (needSpace) output << " ";
        output << "<<";
        needSpace = false;
        newLine = false;
        ++i; // skip next '<'
        continue;
      }

      if (isScopeOpen(c)) {
        if (newLine) output << indent(indentLevel);
        else if (needSpace) output << " ";
        output << c << "\n";
        indentLevel++;
        newLine = true;
        needSpace = false;
      } else if (isScopeClose(c)) {
        indentLevel--;
        output << indent(indentLevel) << c;
        newLine = false;
        needSpace = false;
      } else if (c == ';') {
        output << ";\n";
        newLine = true;
        needSpace = false;
      } else if (c == ',') {
        output << "\n" << indent(indentLevel) << ", ";
        newLine = false;
        needSpace = false;
      } else if (isSeparator(c)) {
        output << "\n" << indent(indentLevel) << c << " ";
        newLine = false;
        needSpace = false;
      } else {
        if (newLine) output << indent(indentLevel);
        else if (needSpace) output << " ";
        output << c;
        newLine = false;
        needSpace = false;
      }
    }

    return output.str();
  }

}
