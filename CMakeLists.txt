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
#
# CMake issue: https://gitlab.kitware.com/cmake/cmake/-/issues/25947
# CMake fix Milestone 3.29.3: https://gitlab.kitware.com/cmake/cmake/-/merge_requests/9471
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Set the build configurations to only Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

project(cxx-reminder)

add_subdirectory(Modules)
    
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.cpp")
    
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${SOURCE_FILES})

add_executable(main README.md ${SOURCE_FILES})

target_link_libraries(main MathLib)

# Set main as the default project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
