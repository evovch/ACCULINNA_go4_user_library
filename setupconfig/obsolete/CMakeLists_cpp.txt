# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/src' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ROOTCLING_HEADERS SetupConfig.h
                      stc_mapping.h
                      )

ROOT_GENERATE_DICTIONARY(G__SetupConfig ${ROOTCLING_HEADERS} LINKDEF SetupConfigLinkDef.h)

# =============================================================================

set(SOURCE_LIB G__SetupConfig.cxx
               SetupConfig.cxx
               )

add_library(SetupConfig SHARED ${SOURCE_LIB})

add_executable(setupconfigtest setupconfig_test.cpp)

# This project libraries
target_link_libraries(setupconfigtest SetupConfig)

# ROOT libraries
target_link_libraries(setupconfigtest Core RIO)
