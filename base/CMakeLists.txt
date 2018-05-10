include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB Support.cxx
               )

add_library(Base SHARED ${SOURCE_LIB})
