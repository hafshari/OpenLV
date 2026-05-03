cmake_minimum_required(VERSION 3.15)
project(lv_stdio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lv_stdio SHARED src/lv_stdio.cpp)

set_target_properties(lv_stdio PROPERTIES
    OUTPUT_NAME "lv_stdio"
    POSITION_INDEPENDENT_CODE ON
)

if(APPLE)
  set_target_properties(lv_stdio PROPERTIES MACOSX_RPATH ON)
endif()
