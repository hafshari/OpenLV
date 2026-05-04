#include <iostream>

#include "lv_stdio.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

/**
 * Shared library for LabVIEW Call Library Function Node testing:
 * writes text to the host process's stdout/stderr using std::cout / std::cerr.
 *
 * macOS: liblv_stdio.dylib / .bundle / lv_stdio.framework — Linux: .so — Windows: .dll
 */

#ifdef _WIN32
#define LV_STDIO_API __declspec(dllexport)
#else
#define LV_STDIO_API __attribute__((visibility("default")))
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
	exit(1);
    }
}

LV_STDIO_API void lv_stdio_write_stderr_nl(const char *text)
{
    if (text != nullptr) {
        std::cerr << text << std::endl;
    }
}

} // extern "C"
