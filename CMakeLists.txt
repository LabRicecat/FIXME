cmake_minimum_required(VERSION 3.9)
project(meow-script 
    VERSION 0.0.0
    LANGUAGES CXX
    DESCRIPTION "A broken esoteric programming language"
)
set(CMAKE_CXX_STANDARD 23)
set(BINARY fixme)
set(SOURCE 
    fixme.cpp
    )

add_executable(${BINARY} ${SOURCE})
install(TARGETS ${BINARY})
