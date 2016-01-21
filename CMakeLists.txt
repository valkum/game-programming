cmake_minimum_required (VERSION 2.8)
project (Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

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

add_subdirectory(src)

# install(FILES assets DESTINATION assets)
