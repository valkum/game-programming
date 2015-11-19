cmake_minimum_required (VERSION 3.0)
project (Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules )
message(STATUS "${CMAKE_MODULE_PATH}")


# ACGL setup
SET(ACGL_OPENGL_SUPPORT CORE_33)
ADD_DEFINITIONS(-DACGL_OPENGL_VERSION_33)
ADD_DEFINITIONS(-DACGL_OPENGL_PROFILE_CORE)


# CMake soll auch in diesen Verzeichnissen weitersuchen
#add_subdirectory(lib)
add_subdirectory(src)

# install(FILES assets DESTINATION assets)