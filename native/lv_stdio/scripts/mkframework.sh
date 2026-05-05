#!/usr/bin/env bash
# Assemble lv_stdio.framework from a freshly built dylib-style Mach-O.
# Usage: mkframework.sh <path/to/lv_stdio> <output_dir_parent>
# The first argument must be the Mach-O named "lv_stdio" (no extension) or we copy & rename.

set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${1:?usage: mkframework.sh <mach-o binary> <framework_parent_dir>}"
PARENT="${2:?usage: mkframework.sh <mach-o binary> <framework_parent_dir>}"
FW="${PARENT}/lv_stdio.framework"
A="${FW}/Versions/A"
HDR_SRC="${ROOT}/include/lv_stdio.h"
HDR_CLFN="${ROOT}/include/lv_stdio_clfn.h"

rm -rf "${FW}"
mkdir -p "${A}/Headers"
cp "${HDR_SRC}" "${A}/Headers/lv_stdio.h"
cp "${HDR_CLFN}" "${A}/Headers/lv_stdio_clfn.h"
cp "${BIN}" "${A}/lv_stdio"
chmod +x "${A}/lv_stdio"

# Current → A
ln -sf A "${FW}/Versions/Current"

# Top-level symlinks (Xcode-style)
ln -sf "Versions/Current/lv_stdio" "${FW}/lv_stdio"
ln -sf "Versions/Current/Headers" "${FW}/Headers"

echo "Created ${FW}"
