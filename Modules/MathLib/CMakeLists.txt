cmake_minimum_required(VERSION 3.28)

add_library(MathLib)

target_sources(MathLib
    PRIVATE
        src/Math.cpp
    PUBLIC
        FILE_SET cxx_mathlib_modules TYPE CXX_MODULES FILES
        interface/Math.ixx
)

target_link_libraries(MathLib PrintLib)
