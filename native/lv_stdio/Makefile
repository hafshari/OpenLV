# Simple build for liblv_stdio (std::cout/std::cerr helpers).
# Unix/macOS: GNU Make 3.81+. Windows: use CMake or install mingw32-make + same toolchain.
#
# macOS extras for LabVIEW "Import Shared Library":
#   make bundle     → build/liblv_stdio.bundle (MH_BUNDLE)
#   make framework  → build/lv_stdio.framework/

INCDIR    := include
BUILD_DIR := build
SRC       := src/lv_stdio.cpp
CXX       ?= c++
CXXFLAGS  ?= -std=c++17 -O2 -Wall -fPIC -I$(INCDIR)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  OUTPUT     := $(BUILD_DIR)/liblv_stdio.dylib
  LINK_FLAGS := -dynamiclib -Wl,-install_name,@rpath/liblv_stdio.dylib
  BUNDLE     := $(BUILD_DIR)/liblv_stdio.bundle
  FW_STAGING := $(BUILD_DIR)/fw_staging/lv_stdio
  FRAMEWORK  := $(BUILD_DIR)/lv_stdio.framework
else
  OUTPUT := $(BUILD_DIR)/liblv_stdio.so
  LINK_FLAGS := -shared
endif

.PHONY: all clean bundle framework

all: $(OUTPUT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LINK_FLAGS) -o $@ $(SRC)

clean:
	rm -rf $(BUILD_DIR)

# --- macOS only (LabVIEW-friendly loadables) ---
ifeq ($(UNAME_S),Darwin)

bundle: $(BUNDLE)

$(BUNDLE): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -bundle -o $@ $(SRC)

# Mach-O for framework interior (dylib with framework-style install_name)
$(FW_STAGING): $(SRC) | $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/fw_staging
	$(CXX) $(CXXFLAGS) -dynamiclib \
	  -Wl,-install_name,@rpath/lv_stdio.framework/Versions/A/lv_stdio \
	  -o $@ $(SRC)

framework: $(FW_STAGING)
	bash scripts/mkframework.sh $(FW_STAGING) $(BUILD_DIR)

macos-all: $(OUTPUT) $(BUNDLE) framework

else

bundle framework macos-all:
	@echo "bundle/framework targets are only available on macOS" >&2
	@exit 1

endif
