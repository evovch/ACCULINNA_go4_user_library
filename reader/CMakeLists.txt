# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/reader' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS Reader.h
                      )

ROOT_GENERATE_DICTIONARY(G__Reader ${ROOTCLING_HEADERS} LINKDEF ReaderLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__Reader.cxx
               Reader.cxx
               )

add_library(Reader SHARED ${SOURCE_LIB})

target_link_libraries(Reader Data SetupConfigCppWrapper)
