cmake_minimum_required(VERSION 3.0)

project(cxx-reminder)

set(SOURCE_FILE_LIST
    src/Main.cpp
    src/DataStructures.cpp
    src/Concurrency.cpp
    src/Cxx11.cpp
    src/Cxx14.cpp
    src/Cxx17.cpp
    src/Cxx20.cpp
    src/Cxx23.cpp)

add_executable(main ${SOURCE_FILE_LIST})
