cmake_minimum_required(VERSION 3.0)
project(TelnetServer CXX)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

# We do want to see make calling things
#set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Needs ncurses to be compiled
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Compilation flags
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 ")

# Inclusion directories
include_directories(./src/ ./include/ ./server/)

# Sources
file(GLOB SRC_FILES ./src/*.cc ./server/*.cc)

# Specify source files
add_executable(telnet_server ${SRC_FILES})

# Link libraries
target_link_libraries(telnet_server ${CURSES_LIBRARIES})
