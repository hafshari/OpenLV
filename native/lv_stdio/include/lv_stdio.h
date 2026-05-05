/**
 * lv_stdio — C API for LabVIEW Call Library Function / Import Shared Library.
 * Implementation is C++ (std::cout / std::cerr); this header is C-safe.
 */
#ifndef LV_STDIO_H
#define LV_STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/** Write text to stdout (no trailing newline); flushes. */
void lv_stdio_write_stdout(const char *text);

/** Write text to stderr (no trailing newline); flushes. */
void lv_stdio_write_stderr(const char *text);

/** Write text to stdout followed by a newline. */
void lv_stdio_write_stdout_nl(const char *text);

/** Write text to stderr followed by a newline. */
void lv_stdio_write_stderr_nl(const char *text);

/**
 * End the current process with the given exit status (0 = success, non-zero = error).
 * This calls the C runtime `exit` and will terminate the whole host (e.g. LabVIEW or your built EXE), not just the diagram.
 */
void lv_stdio_exit(int exit_code);

#ifdef __cplusplus
}
#endif

#endif /* LV_STDIO_H */
