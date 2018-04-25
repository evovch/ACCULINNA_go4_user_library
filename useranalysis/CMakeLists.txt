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

                      data_events/UserEventNew.h
                      data_events/UserEventLeftTele.h
                      data_events/UserEventRightTele.h
                      data_events/UserEventAnnularTele.h
                      data_events/UserEventNeutronDet.h
                      data_events/UserEventBeamDetTof.h
                      data_events/UserEventBeamDetMwpc.h

                      monitoring/UserProcMonitoring.h
                      monitoring/UserHistosMonitoring.h
                      learn/UserEventLearn.h
                      learn/UserProcLearn.h
                      step2/UserEventStep2.h
                      step2/UserProcStep2.h
                      data/BeamDetMWPCDigi.h
                      data/RawMessage.h
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

               data_events/UserEventNew.cxx
               data_events/UserEventLeftTele.cxx
               data_events/UserEventRightTele.cxx
               data_events/UserEventAnnularTele.cxx
               data_events/UserEventNeutronDet.cxx
               data_events/UserEventBeamDetTof.cxx
               data_events/UserEventBeamDetMwpc.cxx

               monitoring/UserProcMonitoring.cxx
               monitoring/UserHistosMonitoring.cxx
               learn/UserEventLearn.cxx
               learn/UserProcLearn.cxx
               step2/UserEventStep2.cxx
               step2/UserProcStep2.cxx
               data/BeamDetMWPCDigi.cxx
               data/RawMessage.cxx
               Support.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})

target_link_libraries(UserAnalysis Go4Analysis)

target_link_libraries(UserAnalysis SetupConfigCppWrapper)
