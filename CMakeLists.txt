cmake_minimum_required(VERSION 3.28)

project(cxx-reminder)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Modules)

set(SOURCE_FILE_LIST
    src/main.cpp
    src/DataStructures.cpp
    src/Algorithms.cpp
    src/Concurrency.cpp
    src/Streams.cpp
    src/Files.cpp
    src/Traits.cpp
    src/Ranges.cpp
    src/Cxx11.cpp
    src/Cxx14.cpp
    src/Cxx17.cpp
    src/Cxx20.cpp)

add_executable(main ${SOURCE_FILE_LIST})

target_link_libraries(main MathLib)
