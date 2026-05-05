#include <cstdlib>
#include <iostream>

#include "lv_stdio.h"

/**
 * Shared library for LabVIEW Call Library Function Node testing:
 * writes text to the host process's stdout/stderr using std::cout / std::cerr.
 *
 * macOS: liblv_stdio.dylib / .bundle / lv_stdio.framework — Linux: .so — Windows: .dll
 */

#ifdef _WIN32
#define LV_STDIO_API __declspec(dllexport)
#define LV_STDIO_NOINLINE __declspec(noinline)
#else
#define LV_STDIO_API __attribute__((visibility("default")))
#define LV_STDIO_NOINLINE __attribute__((noinline))
#endif

extern "C" {

LV_STDIO_API void lv_stdio_write_stdout(const char *text)
{
    if (text != nullptr) {
        std::cout << text << std::flush;
    }
}

LV_STDIO_API void lv_stdio_write_stderr(const char *text)
{
    if (text != nullptr) {
        std::cerr << text << std::flush;
    }
}

LV_STDIO_API void lv_stdio_write_stdout_nl(const char *text)
{
    if (text != nullptr) {
        std::cout << text << std::endl;
    }
}

LV_STDIO_API void lv_stdio_write_stderr_nl(const char *text)
{
    if (text != nullptr) {
        std::cerr << text << std::endl;
    }
}

/**
 * Keep a stable exported symbol for LabVIEW/dlsym: hidden default visibility + explicit export,
 * and noinline so LTO is unlikely to merge this away from its own dylib export entry.
 */
[[noreturn]] LV_STDIO_API LV_STDIO_NOINLINE void lv_stdio_exit(int exit_code)
{
    std::exit(exit_code);
}

} // extern "C"
