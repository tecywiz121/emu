cmake_minimum_required(VERSION 2.8)
project(EMU)

# Add cmake modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EMU_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "-g3 -gdwarf-3 -Wall -Wextra -Wconversion -Werror -Wstrict-aliasing -std=c++11 -pedantic")

add_executable(emu src/main.cpp src/page.cpp)
