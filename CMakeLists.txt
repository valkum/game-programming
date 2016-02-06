cmake_minimum_required (VERSION 2.8)
project (Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()


if(NOT DEFINED ENV{TRAVIS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

set(C_FLAGS_COMMON "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g ${C_FLAGS_COMMON} -fno-strict-aliasing -O3")
set(CMAKE_C_FLAGS "-std=c11 ${C_FLAGS_COMMON}")

set(CMAKE_VERBOSE_MAKEFILE OFF)


# ACGL setup
SET(ACGL_OPENGL_SUPPORT CORE_33)
ADD_DEFINITIONS(-DACGL_OPENGL_VERSION_33)
ADD_DEFINITIONS(-DACGL_OPENGL_PROFILE_CORE)

set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)

#add_custom_command(OUTPUT ${dir} DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/assets" COMMAND -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${dir})
#add_custom_target(${dir} DEPENDS)
# Copy assets



# CMake soll auch in diesen Verzeichnissen weitersuchen
#add_subdirectory(lib)
include_directories(lib/nanovg/src)
add_subdirectory(lib/nanovg)
include_directories(lib/json/src)

if(NOT DEFINED ENV{TRAVIS})
  set(GLFW_BUILD_EXAMPLES OFF)
  set(GLFW_BUILD_TESTS OFF)
  set(GLFW_BUILD_DOCS OFF)
  add_subdirectory(lib/glfw)
  include_directories(lib/glfw/include)
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Werror")



add_subdirectory(src)

# install(FILES assets DESTINATION assets)
