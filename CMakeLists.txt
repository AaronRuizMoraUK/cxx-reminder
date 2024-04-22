cmake_minimum_required(VERSION 3.10)

project(cxx-reminder)

set(SOURCE_FILE_LIST
    src/main.cpp
    src/DataStructures.cpp
    src/Algorithms.cpp
    src/Concurrency.cpp
    src/Streams.cpp
    src/Views.cpp
    src/Files.cpp
    src/Cxx11.cpp
    src/Cxx14.cpp
    src/Cxx17.cpp
    src/Cxx20.cpp)

add_executable(main ${SOURCE_FILE_LIST})

# Use C++ 20 standard
set_property(TARGET main PROPERTY CXX_STANDARD 20)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
