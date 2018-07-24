# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/go4pieces' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS TGo4CompositeEvent.h
                      TGo4EventElement.h
                      TGo4LockGuard.h
                      TGo4Log.h
                      )

ROOT_GENERATE_DICTIONARY(G__Go4pieces ${ROOTCLING_HEADERS} LINKDEF Go4piecesLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__Go4pieces.cxx
               TGo4CompositeEvent.cxx
               TGo4EventElement.cxx
               TGo4LockGuard.cxx
               TGo4Log.cxx
               )

add_library(Go4pieces SHARED ${SOURCE_LIB})
