# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}Go4pieces)

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

ROOT_GENERATE_DICTIONARY(G__${COMPNAME} ${ROOTCLING_HEADERS} LINKDEF Go4piecesLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__${COMPNAME}.cxx
               TGo4CompositeEvent.cxx
               TGo4EventElement.cxx
               TGo4LockGuard.cxx
               TGo4Log.cxx
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}_rdict.pcm DESTINATION lib)
