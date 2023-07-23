cmake_minimum_required(VERSION 3.5)
project(chess_game_cpp)

# Set the C and C++ compilers
set(CMAKE_C_COMPILER C:/msys64/mingw64/bin/gcc.exe)
set(CMAKE_CXX_COMPILER C:/msys64/mingw64/bin/g++.exe)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the source files for the project
set(SOURCES
    src/move_generation.cpp
    src/position.cpp
    src/move_generation_test.cpp
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/tests/output)

# Create the executable
add_executable(move_generation_test ${SOURCES})
add_custom_command(TARGET move_generation_test
                   POST_BUILD
                   COMMAND ${EXECUTABLE_OUTPUT_PATH}/move_generation_test
)

# Link the libraries
# If you have libraries to link, for example a 'chess' library, uncomment the following line
# target_link_libraries(move_generation_test chess)

# Include directories
# If you have include directories, uncomment the following line
# include_directories(${CMAKE_SOURCE_DIR}/include)
