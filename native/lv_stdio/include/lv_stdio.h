/**
 * lv_stdio — C API for LabVIEW Call Library Function / Import Shared Library.
 * Implementation is C++ (std::cout / std::cerr); this header is C-safe.
 *
 * LabVIEW **Import Shared Library** wizard: use `lv_stdio_clfn.h` (no system includes / no `__cplusplus`).
 * In CLFN, `lv_stdio_exit` takes **I32** by value (not `long` / I64). Mach-O names may be `_lv_stdio_*`.
 */
#ifndef LV_STDIO_H
#define LV_STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_stdio_clfn.h"

#ifdef __cplusplus
}
#endif

#endif /* LV_STDIO_H */
