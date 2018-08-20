set(SRCDIR pureC)

set(SOURCE_LIB ${SRCDIR}/functions.c
               ${SRCDIR}/xmlparsing.c
               )

set(SOURCE_EXE ${SRCDIR}/main.c
               )

add_library(SetupConfig SHARED ${SOURCE_LIB})

# ???
set_property(TARGET SetupConfig PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS SetupConfig DESTINATION lib)

# =============================================================================

add_executable(setup_config_test ${SOURCE_EXE})

target_link_libraries(setup_config_test SetupConfig)

# ???
#set_property(TARGET setup_config_test PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS setup_config_test DESTINATION bin)
install(DIRECTORY "." DESTINATION include/setupconfig FILES_MATCHING PATTERN "*.h")