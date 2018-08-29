# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}UserAnalysis)

# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/useranalysis' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS UserAnalysis.h
                      UserParameter.h
                      mesh/MeshDummyEvent.h
                      mesh/MeshProviderProc.h
                      unpacking/UserEventUnpacking.h
                      unpacking/UserProcUnpacking.h
                      unpacking/UserHistosUnpacking.h
                      repacking/UserEventRepacking.h
                      repacking/UserProcRepacking.h
                      repacking/UserHistosRepacking.h
                      learn/UserEventLearn.h
                      learn/UserProcLearn.h
                      digibuilding/UserEventDigiBuilding.h
                      digibuilding/UserProcDigiBuilding.h
                      )

ROOT_GENERATE_DICTIONARY(G__${COMPNAME} ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__${COMPNAME}.cxx
               UserAnalysis.cxx
               UserParameter.cxx
               mesh/MeshDummyEvent.cxx
               mesh/MeshProviderProc.cxx
               unpacking/UserEventUnpacking.cxx
               unpacking/UserProcUnpacking.cxx
               unpacking/UserHistosUnpacking.cxx
               repacking/UserEventRepacking.cxx
               repacking/UserProcRepacking.cxx
               repacking/UserHistosRepacking.cxx
               learn/UserEventLearn.cxx
               learn/UserProcLearn.cxx
               digibuilding/UserEventDigiBuilding.cxx
               digibuilding/UserProcDigiBuilding.cxx
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

# Dependent libraries from this project
target_link_libraries(${COMPNAME} ${PROJECT_PREFIX}Data ${PROJECT_PREFIX}SetupConfigCppWrapper)

# Dependent libraries from go4
target_link_libraries(${COMPNAME} Go4Analysis)

# ???
#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}_rdict.pcm DESTINATION lib)
