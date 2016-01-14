# Zusätzliche include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
if(NOT DEFINED ENV{TRAVIS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

set(C_FLAGS_COMMON "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g ${C_FLAGS_COMMON}")
set(CMAKE_C_FLAGS "-std=c11 ${C_FLAGS_COMMON}")

set(CMAKE_VERBOSE_MAKEFILE ON)

set(IDENTIFIER "de.rwth-aachen.game-programming.heimwerker")
set(COPYRIGHT "All rights reserved. Rudi, Sebo, Kim")
# Hier wird definiert, welche Quellcodedateien zur Programmdatei dazugehören.
# Bei großen Projekten ist eine logische Unterteilung sinnvoll.
set(SOURCE_FILES
    main.cpp
    Game.cpp
    IntroState.cpp
    PlayState.cpp
    Helper.cpp
    TextureLoadStore.cpp
    Model.cpp
    world/Entity.cpp
    world/Object.cpp
    world/SkyDome.cpp
    world/Terrain.cpp
    world/SkyScraper.cpp
    GUI/common.cpp
    GUI/GUIObject.cpp
    GUI/Gui.cpp
    GUI/Button.cpp
    GUI/PerfGraph.cpp
)

FILE(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/assets/shader/*.*")


###############################################################################
#
# all we need for ACGL:
#
ADD_DEFINITIONS(-DACGL_ERROR_LEVEL_EC3)
INCLUDE(${CMAKE_SOURCE_DIR}/lib/acgl/CMakeListsStaticInclude.txt)
#
#
###############################################################################


#
# MacOS X:
#
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(LIBRARIES ${LIBRARIES} -Wl,-framework,Cocoa -Wl,-framework,OpenGL,-framework,IOKit)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#
# Linux:
#
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(LIBRARIES ${LIBRARIES} -lXrandr -lGL -lXi  -pthread -lm -lX11 -lXxf86vm)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#
#
#
###############################################################################



# Die ausführbare Datei wird gegen diese Bibliotheken gelinkt.
#target_link_libraries(Game
#    library
#)

if(UNIX)
	find_package(OpenGL REQUIRED)

	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
    link_directories(${GLFW_LIBRARY_DIRS})

	#find_package(GLM REQUIRED)
	#include_directories(${GLM_INCLUDE_DIR})

	set(REQUIRED_LIBS ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} nanovg)
endif()


#add_executable(Game ${SRCS})


# if(APPLE)
# 	set(MACOSX_BUNDLE_INFO_STRING "@EXE_NAME@ - Version ${VERSION}")
# 	set(MACOSX_RESOURCE_FILES ${CMAKE_SOURCE_DIR}/assets/images/icons.icns)
# 	set(MACOSX_BUNDLE_VERSION ${VERSION})
# 	set(MACOSX_ICON_FILE icons.icns)
# 	set(MACOSX_BUNDLE_BUNDLE_NAME "@EXE_NAME@")
# 	add_executable (Game MACOSX_BUNDLE
# 		${SRCS}
# 		${MACOSX_RESOURCE_FILES})
# endif()


#ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${SHADER_FILES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${REQUIRED_LIBS})

# add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
# 	PRE_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E remove_directory "${dir}/assets"
# )
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
# 		PRE_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${dir}/assets"
# )
if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Linking Assets: ${dir}/assets -> ${CMAKE_SOURCE_DIR}/assets")
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND rm ${dir}/assets && ln -s ../assets ${dir}/assets)

endif()
