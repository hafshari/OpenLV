# OpenLV
OpenLV converts LabVIEW VIs into a textual model and AST using VI Scripting, enabling analysis, pseudocode, UML/dataflow diagrams, fast graphical-to-script transpilation, and AI/LLM insights through a built-in MCP server.

## Cloning
LabVIEW VIs are stored with [Git LFS](https://git-lfs.com). After cloning, run `git lfs install` once, then `git lfs pull` if VIs are missing.

## LabVIEW EXE / stdout / runtime (cross-platform notes)
Executive summary and community patterns (DLL vs proxy CLI, RTE deployment): [docs/labview-exe-stdio-and-runtime.md](docs/labview-exe-stdio-and-runtime.md).

## Native stdio helper (`lv_stdio`)
Experimental shared library (`std::cout` / `std::cerr` via `extern "C"`) for validating CLI-style output when loaded from LabVIEW — see [native/lv_stdio](native/lv_stdio).

Build (after `pip install -r requirements-build.txt` in a venv so `cmake` and `ninja` are on `PATH`):

```bash
cd native/lv_stdio
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On **macOS** the artifact is `build/liblv_stdio.dylib`; on **Linux** `build/liblv_stdio.so`; on **Windows** `build/Release/lv_stdio.dll` (or `build/lv_stdio.dll` depending on generator).
