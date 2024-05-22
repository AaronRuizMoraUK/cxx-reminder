cmake_minimum_required(VERSION 3.29.3)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For C++20 modules. Requires version 3.29.3 to work correctly in VS 2022.
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Set the build configurations to only Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

project(cxx-reminder)

add_subdirectory(Modules)
    
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.cpp")
    
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${SOURCE_FILES})

add_executable(main .gitignore README.md ${SOURCE_FILES})

target_link_libraries(main MathLib)

# Set main as the default project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
