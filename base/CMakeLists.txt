# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}Base)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB Support.cxx
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} LIBRARY DESTINATION lib)
