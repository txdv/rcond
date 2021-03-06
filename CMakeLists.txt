cmake_minimum_required(VERSION 2.8)
PROJECT(rcond)
ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -Wmissing-declarations)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/modules")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(SOURCES
	src/rcond.c
	src/rcond.h
	src/tcp-client-tcp-client.c
	src/tcp-client-tcp-client.h
	src/tcp-server-tcp-server.c
	src/tcp-server-tcp-server.h
	src/misc.h
)

ADD_EXECUTABLE(rcond ${SOURCES})

FIND_PACKAGE(LIBUV REQUIRED)
INCLUDE_DIRECTORIES(${LIBUV_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(rcond ${LIBUV_LIBRARIES})

INSTALL(TARGETS rcond RUNTIME DESTINATION bin)
