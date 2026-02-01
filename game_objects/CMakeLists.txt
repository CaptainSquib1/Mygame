project(game_objects)

add_library(game_objects
    player.cpp player.h
)

target_include_directories(game_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game_objects PUBLIC graphics utils)