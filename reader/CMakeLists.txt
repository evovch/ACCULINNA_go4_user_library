# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/reader' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS Reader.h
                      )

ROOT_GENERATE_DICTIONARY(G__Reader ${ROOTCLING_HEADERS} LINKDEF ReaderLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__Reader.cxx
               Reader.cxx
               )

add_library(Reader SHARED ${SOURCE_LIB})

target_link_libraries(Reader Data SetupConfigCppWrapper)

# ???
#set_property(TARGET Reader PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS Reader LIBRARY DESTINATION lib)
install(DIRECTORY "." DESTINATION include/reader FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libReader.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libReader_rdict.pcm DESTINATION lib)
