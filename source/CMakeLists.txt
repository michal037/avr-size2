#
# MIT License
# Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
#
# STANDARD C11 - GCC
#

cmake_minimum_required(VERSION 3.7)
project(avr-size2)

set(CMAKE_C_STANDARD 11)

# Compiler flags
SET(GCC_COVERAGE_COMPILE_FLAGS "-O2 -Wall") # -O2 -Wall
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Linker flags
SET(GCC_COVERAGE_LINK_FLAGS "-s") # -s
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Sources
set(SOURCE_FILES
        main.c
        avrNameList.h

        # Windows .exe resource - Optional
        # windows/exe.rc
        )
add_executable(avr-size2 ${SOURCE_FILES})
