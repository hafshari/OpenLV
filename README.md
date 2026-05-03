# OpenLV
OpenLV converts LabVIEW VIs into a textual model and AST using VI Scripting, enabling analysis, pseudocode, UML/dataflow diagrams, fast graphical-to-script transpilation, and AI/LLM insights through a built-in MCP server.

## Cloning
LabVIEW VIs are stored with [Git LFS](https://git-lfs.com). After cloning, run `git lfs install` once, then `git lfs pull` if VIs are missing.

## LabVIEW EXE / stdout / runtime (cross-platform notes)
Executive summary and community patterns (DLL vs proxy CLI, RTE deployment): [docs/labview-exe-stdio-and-runtime.md](docs/labview-exe-stdio-and-runtime.md).
