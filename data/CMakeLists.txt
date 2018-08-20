# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/data' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS RawMessage.h
                      DetMessage.h
                      DetEventStation.h
                      DetEventDetector.h
                      DetEventCommon.h
                      DetEventFull.h
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
               DetEventFull.cxx
               )

add_library(Data SHARED ${SOURCE_LIB})

target_link_libraries(Data Tree Base SetupConfigCppWrapper Go4pieces)

#set_property(TARGET Data PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS Data LIBRARY DESTINATION lib)
install(DIRECTORY "." DESTINATION include/data FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libData.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libData_rdict.pcm DESTINATION lib)
