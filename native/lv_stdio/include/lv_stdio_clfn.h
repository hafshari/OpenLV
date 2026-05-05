/**
 * Minimal C declarations for Call Library Function / LabVIEW Import Shared Library.
 * No system includes and no C++ guards — use this file path in the import wizard.
 * (Parameter types: I32 for exit_code, C string for char* — see main lv_stdio.h comments.)
 */
#ifndef LV_STDIO_CLFN_H
#define LV_STDIO_CLFN_H

void lv_stdio_write_stdout(const char *text);
void lv_stdio_write_stderr(const char *text);
void lv_stdio_write_stdout_nl(const char *text);
void lv_stdio_write_stderr_nl(const char *text);
#if defined(__cplusplus)
[[noreturn]]
#endif
void lv_stdio_exit(int exit_code);

#endif /* LV_STDIO_CLFN_H */
