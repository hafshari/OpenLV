cmake_minimum_required(VERSION 3.15)
project(lv_stdio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lv_stdio SHARED src/lv_stdio.cpp)
target_include_directories(lv_stdio
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(lv_stdio PROPERTIES
    OUTPUT_NAME "lv_stdio"
    POSITION_INDEPENDENT_CODE ON
)

if(APPLE)
  set_target_properties(lv_stdio PROPERTIES MACOSX_RPATH ON)

  # lv_stdio.framework — Xcode-style layout for LabVIEW Import Shared Library
  add_library(lv_stdio_framework SHARED src/lv_stdio.cpp)
  target_include_directories(lv_stdio_framework
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
  set_target_properties(lv_stdio_framework PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION A
    MACOSX_FRAMEWORK_IDENTIFIER com.openlv.lv_stdio
    PUBLIC_HEADER include/lv_stdio.h
    OUTPUT_NAME lv_stdio
    POSITION_INDEPENDENT_CODE ON
  )
endif()
