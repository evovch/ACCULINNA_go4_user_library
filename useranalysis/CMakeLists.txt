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
                      monitoring/UserEventMonitoring.h
                      monitoring/UserProcMonitoring.h
                      monitoring/UserHistosMonitoring.h
                      learn/UserEventLearn.h
                      learn/UserProcLearn.h
                      data/RawMessage.h
                      data/DetMessage.h
                      data/DetEventFull.h
                      data/DetEventCommon.h
                      data/DetEventDetector.h
                      data/DetEventStation.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserAnalysis ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__UserAnalysis.cxx
               UserAnalysis.cxx
               UserParameter.cxx
               mesh/MeshDummyEvent.cxx
               mesh/MeshProviderProc.cxx
               unpacking/UserEventUnpacking.cxx
               unpacking/UserProcUnpacking.cxx
               unpacking/UserHistosUnpacking.cxx
               monitoring/UserEventMonitoring.cxx
               monitoring/UserProcMonitoring.cxx
               monitoring/UserHistosMonitoring.cxx
               learn/UserEventLearn.cxx
               learn/UserProcLearn.cxx
               data/RawMessage.cxx
               data/DetMessage.cxx
               data/DetEventFull.cxx
               data/DetEventCommon.cxx
               data/DetEventDetector.cxx
               data/DetEventStation.cxx
               Support.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})

target_link_libraries(UserAnalysis Go4Analysis)

target_link_libraries(UserAnalysis SetupConfigCppWrapper)
