#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #define ZENSEP_EXPORT __declspec(dllexport)
#else
  #define ZENSEP_EXPORT
#endif

// C API exports - minimal interface
ZENSEP_EXPORT void print_build_info(void);

// Core formatting function - C compatible
// Returns: 0 = success, positive = error code
ZENSEP_EXPORT int format_file(const char* input_filename, 
                             const char* output_filename,  // NULL for stdout
                             const char* lines_range,      // NULL or "start:end"
                             int dry_run);                 // 0 = false, 1 = true

#ifdef __cplusplus
}
#endif
