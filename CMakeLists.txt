cmake_minimum_required(VERSION 3.28)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ISSUE: VS 2022 IntelliSense not working when importing C++20 modules.
#
# For Visual Studio 2022 IntelliSense to work with modules correctly
# the property "C/C++ > General > scan sources for module dependencies"
# needs to be enabled manually in all projects after the solution has been generated.
#
# CMake 3.28 provides CMAKE_CXX_SCAN_FOR_MODULES, 
# but this enables the property on each source file and not in the project.
# IntelliSense seems to only work when the property is enabled in the project.
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(cxx-reminder)

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
