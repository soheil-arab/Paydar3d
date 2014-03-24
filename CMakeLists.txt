cmake_minimum_required(VERSION 2.6)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
project(Robotoos CXX)
set(PACKAGE_VERSION "0.01")
########## add configs ########## 
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/robotoos_config.h.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/robotoos_config.h)

########## add subdirectories ############
add_subdirectory(src)

