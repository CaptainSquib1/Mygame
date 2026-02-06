project(utils)

add_library(utils
        vec.cpp vec.h
        physics.cpp physics.h
)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})