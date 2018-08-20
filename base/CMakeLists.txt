include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB Support.cxx
               )

add_library(Base SHARED ${SOURCE_LIB})

#set_property(TARGET Base PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS Base LIBRARY DESTINATION lib)
install(DIRECTORY "." DESTINATION include/base FILES_MATCHING PATTERN "*.h")