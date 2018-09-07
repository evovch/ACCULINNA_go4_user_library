set(COMPNAME setup_config_cpp_test)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_EXE setupConfigCppTest.cpp
               )

add_executable(${COMPNAME} ${SOURCE_EXE})

target_link_libraries(${COMPNAME} Core RIO ${PROJECT_PREFIX}SetupConfigCppWrapper)

# ???
#set_property(TARGET ${COMPNAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS ${COMPNAME} DESTINATION bin)
