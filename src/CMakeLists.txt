# Zusätzliche include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Hier wird definiert, welche Quellcodedateien zur Programmdatei dazugehören.
# Bei großen Projekten ist eine logische Unterteilung sinnvoll.
set(game_SRCS
    main.cpp
)

# Die auführbare Datei. Anstatt der Variablen können
# auch noch zusätzliche Dateien angegeben werden
add_executable (Game ${game_SRCS})

# Die ausführbare Datei wird gegen diese Bibliotheken gelinkt.
#target_link_libraries(Game
#    library
#)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Game ${GLFW_LIBRARIES})

# Das Programm soll in den folgenden Unterordnern
# installiert werden.
install(TARGETS Game
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)