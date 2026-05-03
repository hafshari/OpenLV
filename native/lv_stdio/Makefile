# Simple build for liblv_stdio (std::cout/std::cerr helpers).
# Unix/macOS: GNU Make 3.81+. Windows: use CMake or install mingw32-make + same toolchain.

BUILD_DIR := build
SRC       := src/lv_stdio.cpp
CXX       ?= c++
CXXFLAGS  ?= -std=c++17 -O2 -Wall -fPIC

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  OUTPUT     := $(BUILD_DIR)/liblv_stdio.dylib
  LINK_FLAGS := -dynamiclib -Wl,-install_name,@rpath/liblv_stdio.dylib
else
  OUTPUT     := $(BUILD_DIR)/liblv_stdio.so
  LINK_FLAGS := -shared
endif

.PHONY: all clean

all: $(OUTPUT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LINK_FLAGS) -o $@ $(SRC)

clean:
	rm -rf $(BUILD_DIR)
