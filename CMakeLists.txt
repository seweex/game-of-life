cmake_minimum_required (VERSION 3.18)
project (game-of-life VERSION 1.0 LANGUAGES C CXX)

set (SOURCES src/main.cpp)
set (HEADERS
    include/game.h
    include/window.h
    include/field.h
    include/settings.h
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties (${PROJECT_NAME} PROPERTIES
    LANGUAGE              CXX
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED YES
)

find_package (SDL2 CONFIG REQUIRED)
target_link_libraries (${PROJECT_NAME} PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)