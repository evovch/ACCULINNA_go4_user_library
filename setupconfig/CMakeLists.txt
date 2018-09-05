# Name of the project component which must be constructed as project prefix + some name
set(COMPNAME ${PROJECT_PREFIX}SetupConfig)

set(SRCDIR pureC)

set(SOURCE_LIB ${SRCDIR}/functions.c
               ${SRCDIR}/xmlparsing.c
               )

set(SOURCE_EXE ${SRCDIR}/main.c
               )

add_library(${COMPNAME} SHARED ${SOURCE_LIB})

# ???
set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} DESTINATION lib)

# =============================================================================

add_executable(setup_config_test ${SOURCE_EXE})

target_link_libraries(setup_config_test ${PROJECT_PREFIX}SetupConfig)

# ???
#set_property(TARGET setup_config_test PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS setup_config_test DESTINATION bin)
install(DIRECTORY "." DESTINATION include/setupconfig FILES_MATCHING PATTERN "*.h")
