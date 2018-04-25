# =============================================================================

cmake_minimum_required(VERSION 3.5)

# Give your project a name
project(go4_5H_processor)

# Switch in on for more verbose make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

# =============================================================================
# External packages

# See: https://root.cern.ch/how/integrate-root-my-project-cmake

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Go4
if (DEFINED ENV{GO4SYS})
    message("Go4 found at $ENV{GO4SYS}")
    include_directories($ENV{GO4SYS}/include)
    link_directories($ENV{GO4SYS}/lib)
else()
    message(FATAL_ERROR "Go4 not found! Please, run go4login or define the GO4SYS env. variable.")
endif()

# =============================================================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
    # -std=c++11 -g -Wall -Wextra -Wpedantic
endif()

# =============================================================================

add_subdirectory(useranalysis)
add_subdirectory(setupconfig)
add_subdirectory(setupconfigcppwrapper)
