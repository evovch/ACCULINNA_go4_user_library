# =============================================================================

cmake_minimum_required(VERSION 3.5)

# Give your project a name
project(ACCULINNA_go4_user_library)
set(PROJECT_PREFIX AccDaq)

# Switch it on for more verbose make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

#=============================================================================

# Options
set (READER_MODE OFF
     CACHE BOOL "Compile library only for output files reading")

# =============================================================================
# External packages

# See: https://root.cern.ch/how/integrate-root-my-project-cmake

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Go4
if (NOT READER_MODE)
    if (DEFINED ENV{GO4SYS})
        message("Go4 found at $ENV{GO4SYS}")
        include_directories($ENV{GO4SYS}/include)
        link_directories($ENV{GO4SYS}/lib)
    else()
        message(FATAL_ERROR "Go4 not found! Please, run go4login or define the GO4SYS env. variable. In case you want to compile in the 'Reader mode' which does not require Go4, please, pass the -DREADER_MODE=ON flag to cmake.")
    endif()
endif()

# =============================================================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
    # -std=c++11 -g -pg -Wall -Wextra -Wpedantic
    # set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -pg")
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -pg")
endif()

# =============================================================================

# TODO uncomment certain flags if you want to see more detailed output
# Beware that A LOT of text will be generated. Don't run more than 10-100 events.

#add_definitions(-DDEBUGUNPACKING)
#add_definitions(-DDEBUGRAWMON)
#add_definitions(-DDEBUGREPACKING)
#add_definitions(-DDEBUGADVMON)
#add_definitions(-DDEBUGBEAMDETMON)
#add_definitions(-DDEBUGLEARN)
#add_definitions(-DDEBUGDIGIBUILDING)

# =============================================================================

add_subdirectory(base)
add_subdirectory(go4pieces)
add_subdirectory(data)
add_subdirectory(setupconfig)
add_subdirectory(setupconfigcppwrapper)
if (NOT READER_MODE)
    add_subdirectory(useranalysis)
endif()
add_subdirectory(reader)
add_subdirectory(tests)
