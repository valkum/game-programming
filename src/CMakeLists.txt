# Zusätzliche include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
set(C_FLAGS_COMMON "-Wall -Werror -pedantic")
set(CMAKE_CXX_FLAGS "--std=c++14 ${C_FLAGS_COMMON}")

set(IDENTIFIER "de.rwth-aachen.game-programming.heimwerker")
set(COPYRIGHT "All rights reserved. Rudi, Sebo, Kim")
# Hier wird definiert, welche Quellcodedateien zur Programmdatei dazugehören.
# Bei großen Projekten ist eine logische Unterteilung sinnvoll.
set(SRCS
    main.cpp
    game.cpp
    introstate.cpp
)


# Die ausführbare Datei wird gegen diese Bibliotheken gelinkt.
#target_link_libraries(Game
#    library
#)

if(UNIX)
	find_package(OpenGL REQUIRED)

	find_package(pkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})

	find_package(GLM REQUIRED)
	include_directories(${GLM_INCLUDE_DIR})

	set(REQUIRED_LIBS ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
endif()

add_executable(Game ${SRCS})


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



target_link_libraries(Game ${REQUIRED_LIBS})
