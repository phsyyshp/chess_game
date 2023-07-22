cmake_minimum_required(VERSION 3.0)
project(chess_game_cpp)

# Set the source files for the project
set(SOURCES
    move_generation.cpp
    position.cpp
    test/move_generation_test.cpp
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)

# Create the executable
add_executable(move_generation_test ${SOURCES})
