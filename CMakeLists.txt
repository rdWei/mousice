cmake_minimum_required(VERSION 3.10)
project(mousice C)

set(CMAKE_C_STANDARD 11)

# Aggiungi la directory include per i file header
include_directories(include)

# Trova le librerie CURL e NCURSES
find_package(CURL REQUIRED)

# Usa pkg-config per trovare ncurses
find_package(PkgConfig REQUIRED)

# Trova la libreria MPV
find_path(MPV_INCLUDE_DIR mpv/client.h)
find_library(MPV_LIBRARY NAMES mpv PATHS /usr/local/lib /usr/lib)

if (NOT MPV_INCLUDE_DIR OR NOT MPV_LIBRARY)
    message(FATAL_ERROR "MPV library not found")
endif()

include_directories(${MPV_INCLUDE_DIR})
include_directories(${NCURSES_INCLUDE_DIRS})

# Aggiungi l'eseguibile e specifica i file sorgente
add_executable(mousice src/caparezza.c src/scrape.c src/ui.c src/main.c)

# Collega le librerie necessarie
target_link_libraries(mousice ${CURL_LIBRARIES} ${MPV_LIBRARY} ${NCURSES_LIBRARIES})

