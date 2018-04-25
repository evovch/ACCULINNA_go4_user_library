set(SRCDIR pureC)

set(SOURCE_LIB ${SRCDIR}/functions.c
               ${SRCDIR}/xmlparsing.c
               )

set(SOURCE_EXE ${SRCDIR}/main.c
               )

add_library(SetupConfig SHARED ${SOURCE_LIB})

add_executable(setup_config_test ${SOURCE_EXE})

target_link_libraries(setup_config_test SetupConfig)
