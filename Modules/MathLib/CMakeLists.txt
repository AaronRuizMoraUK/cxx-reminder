cmake_minimum_required(VERSION 3.29.3)

add_library(MathLib)

file(GLOB_RECURSE MATHLIB_INTERFACE_FILES
    "${CMAKE_SOURCE_DIR}/Modules/MathLib/interface/*.ixx")

file(GLOB_RECURSE MATHLIB_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/Modules/MathLib/src/*.cpp")
    
source_group(TREE "${CMAKE_SOURCE_DIR}/Modules/MathLib" FILES ${MATHLIB_INTERFACE_FILES} ${MATHLIB_SOURCE_FILES})

target_sources(MathLib
    PRIVATE
        ${MATHLIB_SOURCE_FILES}
    PUBLIC
        FILE_SET cxx_mathlib_modules TYPE CXX_MODULES
        FILES
            ${MATHLIB_INTERFACE_FILES}
)

target_link_libraries(MathLib PrintLib)
