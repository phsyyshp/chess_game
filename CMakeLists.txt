cmake_minimum_required(VERSION 3.5)
project(chess_game_cpp)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimizer flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -march=native")

# Main application source files
set(MAIN_SOURCES
    src/move_generation.cpp
    src/position.cpp
    src/loader.cpp
    src/Move.cpp
    src/tables.cpp
    src/utilities.cpp
    src/bitoperations.cpp
    src/evaluation.cpp
    src/search.cpp
    src/movelist.cpp
    src/UCI.cpp
    src/gameState.cpp
    src/moveToStr.cpp
    src/zobrist.cpp
    src/tt.cpp
    src/main.cpp 
)

# Unit test source files
set(TEST_SOURCES
    tests/perftTest.cpp
    tests/unitTests.cpp
    src/move_generation.cpp
    src/position.cpp
    src/loader.cpp
    src/Move.cpp
    src/tables.cpp
    src/utilities.cpp
    src/bitoperations.cpp
    src/evaluation.cpp
    src/search.cpp
    src/movelist.cpp
    src/UCI.cpp
    src/gameState.cpp
    src/zobrist.cpp
    src/moveToStr.cpp
    src/tt.cpp
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Create the main executable
add_executable(engine_0_2_1_alpha ${MAIN_SOURCES})

# Create the unit tests executable
add_executable(unitTests ${TEST_SOURCES})

# Locate GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
target_link_libraries(unitTests ${GTEST_LIBRARIES} pthread)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)