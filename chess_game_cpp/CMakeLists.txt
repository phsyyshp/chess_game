cmake_minimum_required(VERSION 3.5)
project(chess_game_cpp)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conditional settings for Windows and WSL/Linux
# if(WIN32)
#     # Set the C and C++ compilers for MinGW
#     set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
#     set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
# endif()

# Set the source files for the main executable
set(SOURCES
    src/move_generation.cpp
    src/position.cpp
    src/loader.cpp
    # tests/move_generation_test.cpp
    tests/loader_test.cpp
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Create the main executable
# add_executable(move_generation_test ${SOURCES})
add_executable(loader_test ${SOURCES})
 

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Post-build command to run the test
# add_custom_command(TARGET move_generation_test
#                    POST_BUILD
#                    COMMAND ${EXECUTABLE_OUTPUT_PATH}/move_generation_test
# )
add_custom_command(TARGET loader_test
                   POST_BUILD
                   COMMAND ${EXECUTABLE_OUTPUT_PATH}/loader_test
)
 
