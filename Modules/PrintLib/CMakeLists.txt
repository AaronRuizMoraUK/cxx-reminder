cmake_minimum_required(VERSION 3.28)

add_library(PrintLib)

file(GLOB_RECURSE MATHLIB_INTERFACE_FILES
    "${CMAKE_SOURCE_DIR}/Modules/PrintLib/interface/*.ixx")

file(GLOB_RECURSE MATHLIB_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Modules/PrintLib/src/*.cpp")
    
source_group(TREE "${CMAKE_SOURCE_DIR}/Modules/PrintLib" FILES ${MATHLIB_INTERFACE_FILES} ${MATHLIB_SOURCE_FILES})

target_sources(PrintLib
    PRIVATE
        ${MATHLIB_SOURCE_FILES}
    PUBLIC
        FILE_SET cxx_printlib_modules TYPE CXX_MODULES
        FILES
            ${MATHLIB_INTERFACE_FILES}
)
