cmake_minimum_required(VERSION 3.28)

add_library(PrintLib)

target_sources(PrintLib
    PRIVATE
        src/Print.cpp
    PUBLIC
        FILE_SET cxx_printlib_modules TYPE CXX_MODULES FILES
        interface/Print.ixx
)
