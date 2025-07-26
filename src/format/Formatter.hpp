#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace zensep {

  class Formatter {
  public:
    using pointer = std::shared_ptr<Formatter>;
    struct Options {
      std::optional<std::pair<size_t, size_t>> line_range;
    };
    Formatter(Options const& options);
    using FormatResult = std::string;
    virtual FormatResult format(const std::string& source) = 0;
  protected:
    Options m_options;
  };

  class NoOpFormatter : public Formatter {
  public:
    NoOpFormatter(Options const& options) : Formatter(options) {}
    virtual FormatResult format(const std::string& source) override;
  };

  template <class Fmtr>
  Formatter::pointer make_formatter(Formatter::Options const& options) {
    return std::make_shared<Fmtr>(options);
  }

}
