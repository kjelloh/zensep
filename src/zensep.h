#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define ZENSEP_EXPORT __declspec(dllexport)
#else
  #define ZENSEP_EXPORT
#endif

ZENSEP_EXPORT void zensep();
ZENSEP_EXPORT void zensep_print_vector(const std::vector<std::string> &strings);
