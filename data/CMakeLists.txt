# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/useranalysis/data' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS RawMessage.h
                      DetMessage.h
                      DetEventStation.h
                      DetEventDetector.h
                      DetEventCommon.h
                      )

ROOT_GENERATE_DICTIONARY(G__Data ${ROOTCLING_HEADERS} LINKDEF DataLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__Data.cxx
               RawMessage.cxx
               DetMessage.cxx
               DetEventStation.cxx
               DetEventDetector.cxx
               DetEventCommon.cxx
               )

add_library(Data SHARED ${SOURCE_LIB})

target_link_libraries(Data Base)
