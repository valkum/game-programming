project(nanovg)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

OPTION(NANOVG_BUILD_SHARED "Build NanoVG as a shared library?" OFF)
if(APPLE AND NANOVG_BUILD_SHARED)
    set(CMAKE_MACOSX_RPATH ON)
endif()

if(NANOVG_BUILD_SHARED)
  set(NANOVG_LIBRARY_TYPE "SHARED")
else()
  set(NANOVG_LIBRARY_TYPE "STATIC")
endif()
set_source_files_properties(src/nanovg.c PROPERTIES COMPILE_FLAGS -Wno-unused-result)

INCLUDE_DIRECTORIES(src)

add_library(nanovg ${NANOVG_LIBRARY_TYPE} 
    src/fontstash.h
    src/nanovg_gl.h
    src/nanovg_gl_utils.h
    src/stb_image.h
    src/stb_truetype.h
    src/nanovg.h src/nanovg.c 
  )