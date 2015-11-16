cmake_minimum_required (VERSION 3.0)
project (Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules )
message(STATUS "${CMAKE_MODULE_PATH}")


# CMake soll auch in diesen Verzeichnissen weitersuchen
#add_subdirectory(lib)
add_subdirectory(src)

# install(FILES assets DESTINATION assets)