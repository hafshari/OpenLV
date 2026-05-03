# OpenLV
OpenLV converts LabVIEW VIs into a textual model and AST using VI Scripting, enabling analysis, pseudocode, UML/dataflow diagrams, fast graphical-to-script transpilation, and AI/LLM insights through a built-in MCP server.

## Cloning
LabVIEW VIs are stored with [Git LFS](https://git-lfs.com). After cloning, run `git lfs install` once, then `git lfs pull` if VIs are missing.

## LabVIEW EXE / stdout / runtime (cross-platform notes)
Executive summary and community patterns (DLL vs proxy CLI, RTE deployment): [docs/labview-exe-stdio-and-runtime.md](docs/labview-exe-stdio-and-runtime.md).

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

Header for the wizard: [`native/lv_stdio/include/lv_stdio.h`](native/lv_stdio/include/lv_stdio.h).

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
