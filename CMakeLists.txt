# First attempt at CMake.

cmake_minimum_required(VERSION 2.6)
project(mod_server)

option(VERBOSE "Display additional info when generating files" ON)

include_directories(.)

if(WIN32)
  add_definitions(-DWIN32 -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -DFD_SETSIZE=128)
  set(CMAKE_CXX_FLAGS "-W4 -WX -EHsc -wd4127")
else()
  add_definitions(-DUNIX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++ -std=c++11 -g -O0")
  set(UNIX_SPECIFIC_LIBS dl)
endif()

if (VERBOSE)
  message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(base)
add_subdirectory(socket)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(OUT_NAME mod_server)

set(SERVER_CORE_SRC "server_core/ip_endpoint.cpp"
                    "server_core/ip_endpoint.h"
                    "server_core/server.cpp"
                    "server_core/server.h"
                    "server_core/session.cpp"
                    "server_core/session.h"
                    "server_core/tcp_session.cpp"
                    "server_core/tcp_session.h")
                    
set(SERVER_PLUGIN_SRC "server_plugin/plugin_loader.cpp"
                      "server_plugin/plugin_loader.h"
                      "server_plugin/server_plugin.cpp"
                      "server_plugin/server_plugin.h")

add_executable(${OUT_NAME} main.cpp ${SERVER_CORE_SRC} ${SERVER_PLUGIN_SRC})
target_link_libraries(${OUT_NAME} base socket ${UNIX_SPECIFIC_LIBS})

# Build plugins.
add_subdirectory(server_plugin)
