cmake_minimum_required(VERSION 3.5)
project(chess_game_cpp)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Profiling flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# Optimizer flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -march=native")


# Set the source files for the main executable
set(SOURCES
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
    # src/moveValidator.cpp
    tests/perftTest.cpp
    # tests/gameLoopTest.cpp
    # tests/unitTests.cpp
     
 
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Create the main executable
add_executable(perftTest ${SOURCES})
# add_executable(gameLoopTest ${SOURCES})
# add_executable(unitTests ${SOURCES})
 
 # Locate GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
# target_link_libraries(unitTests ${GTEST_LIBRARIES} pthread)
 
 

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
