# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}SetupConfig)

# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/setupconfig' in our case)
# is not added to the include_directories list.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pureC)
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS structs.h
                      functions.h
                      xmlparsing.h
                      )

ROOT_GENERATE_DICTIONARY(G__${COMPNAME} ${ROOTCLING_HEADERS} LINKDEF SetupConfigLinkDef.h)

# =============================================================================
# Dynamic library

set(SOURCE_LIB G__${COMPNAME}.cxx
               functions.cpp
               xmlparsing.cpp
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

# ???
#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}_rdict.pcm DESTINATION lib)
install(DIRECTORY "." DESTINATION include/setupconfig FILES_MATCHING PATTERN "*.h")

# =============================================================================
# Executable

set(SOURCE_EXE main.cpp
               )

add_executable(setup_config_test ${SOURCE_EXE})

target_link_libraries(setup_config_test Core ${PROJECT_PREFIX}SetupConfig)

# ???
#set_property(TARGET setup_config_test PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS setup_config_test DESTINATION bin)
