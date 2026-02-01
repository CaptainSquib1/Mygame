project(world)

add_library(world
        world.cpp world.h
)

target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(world PUBLIC SDL3::SDL3 game_objects)