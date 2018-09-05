# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}Data)

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

ROOT_GENERATE_DICTIONARY(G__${COMPNAME} ${ROOTCLING_HEADERS} LINKDEF DataLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__${COMPNAME}.cxx
               RawMessage.cxx
               DetMessage.cxx
               DetEventStation.cxx
               DetEventDetector.cxx
               DetEventCommon.cxx
               DetEventFull.cxx
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

target_link_libraries(${COMPNAME} Tree ${PROJECT_PREFIX}Base ${PROJECT_PREFIX}SetupConfigCppWrapper ${PROJECT_PREFIX}Go4pieces)

#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}_rdict.pcm DESTINATION lib)
install(DIRECTORY "." DESTINATION include/data FILES_MATCHING PATTERN "*.h")
