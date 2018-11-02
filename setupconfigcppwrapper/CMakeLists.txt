# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}SetupConfigCppWrapper)

# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/setupconfigcppwrapper' in our case)
# is not added to the include_directories list.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS SetupConfiguration.h
                      ElectronicsChars.h
                      )

ROOT_GENERATE_DICTIONARY(G__${COMPNAME} ${ROOTCLING_HEADERS} LINKDEF SetupConfigCppWrapperLinkDef.h)

# =============================================================================

set(SOURCE_LIB G__${COMPNAME}.cxx
               SetupConfiguration.cxx
               ElectronicsChars.cxx
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

target_link_libraries(${COMPNAME} ${PROJECT_PREFIX}SetupConfig)

#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${COMPNAME}_rdict.pcm DESTINATION lib)
install(DIRECTORY "." DESTINATION include/setupconfigcppwrapper FILES_MATCHING PATTERN "*.h")
