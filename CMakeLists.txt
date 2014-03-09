cmake_minimum_required(VERSION 2.6)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
project(Robotoos CXX)
set(PACKAGE_VERSION "0.01")
########## check for headerfiles/libraries ##########
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

set(RCSSNET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet)
set(SPARK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set (rcssnet_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/addr.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/addr.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/exception.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/exception.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/handler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/handler.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/iosocketstream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/isocketstream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/osocketstream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/socket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/socket.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/socketstreambuf.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/tcpsocket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/tcpsocket.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/udpsocket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/rcssnet/udpsocket.hpp
)
set(CMAKE_CXX_FLAGS " -pg -I/usr/include/eigen3 -lorocos-kdl")
########## add configs ########## 
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/robotoos_config.h.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/robotoos_config.h)

########## add subdirectories ############
add_subdirectory(src)

