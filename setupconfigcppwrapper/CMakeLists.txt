# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/setupconfigcppwrapper' in our case)
# is not added to the include_directories list.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS SetupConfiguration.h
                      )

ROOT_GENERATE_DICTIONARY(G__SetupConfigCppWrapper ${ROOTCLING_HEADERS} LINKDEF SetupConfigCppWrapperLinkDef.h)

# =============================================================================

set(SOURCE_LIB G__SetupConfigCppWrapper.cxx
               SetupConfiguration.cxx
               )

add_library(SetupConfigCppWrapper SHARED ${SOURCE_LIB})

target_link_libraries(SetupConfigCppWrapper SetupConfig)

#set_property(TARGET SetupConfigCppWrapper PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS SetupConfigCppWrapper DESTINATION lib)
install(DIRECTORY "." DESTINATION include/setupconfigcppwrapper FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libSetupConfigCppWrapper.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libSetupConfigCppWrapper_rdict.pcm DESTINATION lib)
