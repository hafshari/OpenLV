# OpenLV
OpenLV converts LabVIEW VIs into a textual model and AST using VI Scripting, enabling analysis, pseudocode, UML/dataflow diagrams, fast graphical-to-script transpilation, and AI/LLM insights through a built-in MCP server.

## Cloning
LabVIEW VIs are stored with [Git LFS](https://git-lfs.com). After cloning, run `git lfs install` once, then `git lfs pull` if VIs are missing.

## LabVIEW EXE / stdout / runtime (cross-platform notes)
Executive summary and community patterns (DLL vs proxy CLI, RTE deployment): [docs/labview-exe-stdio-and-runtime.md](docs/labview-exe-stdio-and-runtime.md).

## VIPM CLI (research)
VIPM CLI docs summary, macOS vs Preview installers, Homebrew/Docker notes, and upstream requests: [docs/vipm-cli-research.md](docs/vipm-cli-research.md).

## Native stdio helper (`lv_stdio`)
Experimental shared library (`std::cout` / `std::cerr` via `extern "C"`) for validating CLI-style output when loaded from LabVIEW — see [native/lv_stdio](native/lv_stdio).

**CMake + Ninja** (after `pip install -r requirements-build.txt` so `cmake` / `ninja` are on `PATH`):

```bash
cd native/lv_stdio
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Make** (macOS / Linux only; no extra Python deps):

```bash
cd native/lv_stdio
make
```

Use `make clean` to remove `native/lv_stdio/build/`.

On **macOS** the artifact is `build/liblv_stdio.dylib`; on **Linux** `build/liblv_stdio.so`; on **Windows** use CMake — `Makefile` targets Unix-like toolchains only.

### macOS: `.bundle` and `.framework` (LabVIEW *Import Shared Library*)

Import Shared Library wizard (no system includes): [`native/lv_stdio/include/lv_stdio_clfn.h`](native/lv_stdio/include/lv_stdio_clfn.h). Full API wrapper: [`lv_stdio.h`](native/lv_stdio/include/lv_stdio.h).

**Make** (from `native/lv_stdio`):

```bash
make bundle      # → build/liblv_stdio.bundle (MH_BUNDLE)
make framework   # → build/lv_stdio.framework/ (Versions/A, Headers, symlinks)
make macos-all   # dylib + bundle + framework
```

**CMake** (also emits `build/lv_stdio.framework` on Apple when you build the `lv_stdio_framework` target):

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target lv_stdio_framework
# Framework: build/lv_stdio.framework
```

Quick symbol check: `nm -gU build/liblv_stdio.bundle | grep lv_stdio`

**Note:** Prefer **`.bundle`** for a single file; use **`.framework`** when you want the standard macOS layout. Local dev typically needs **no code signing**; gatekeeper is a separate concern for distribution.

### LabVIEW CLFN / Import Shared Library (what we learned)

- **Header for the wizard:** Use [`lv_stdio_clfn.h`](native/lv_stdio/include/lv_stdio_clfn.h) only (no `stdint.h`, no `#ifdef __cplusplus`). Set the wizard **include path** to the `native/lv_stdio/include` directory—not the macOS SDK.
- **Library path on macOS:** Point the Call Library Function node at a real Mach-O: e.g. `…/build/liblv_stdio.dylib` or `…/build/lv_stdio.framework/lv_stdio` (the binary inside the bundle), not the `.framework` directory alone.
- **Symbol names:** Exported C symbols are often `_lv_stdio_*` in `nm -gU`. If LabVIEW reports “function not found,” try the leading underscore or confirm `nm` on the **same file** LabVIEW loads.
- **`lv_stdio_exit`:** Declare as `void lv_stdio_exit(int exit_code)` / **I32, pass by value**—not `long` / I64 (on 64-bit macOS, `long` is 64-bit).
- **Build / linker behavior:** The library is built with **`-fvisibility=hidden`** (Make/CMake) so only `LV_STDIO_API` symbols export; that keeps the dynamic symbol table predictable for `dlsym`. **`lv_stdio_exit` is marked `noinline`** so it stays a distinct exported entry point (helps the Import wizard and loaders; pair with the visibility pattern above). **`[[noreturn]]`** on the declaration (C++) documents that the process ends via `std::exit`.
