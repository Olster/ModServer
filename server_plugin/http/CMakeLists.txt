cmake_minimum_required(VERSION 2.6)
project(http)

add_definitions(-DPLUGIN_IMPLEMENTATION)
#include_directories(../..)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/plugins)

add_library(http_plugin SHARED http_handler.cpp http_handler.h
                               http_plugin.cpp
                               request.cpp request.h
                               response.cpp response.h 
                               ../../plugin_api/plugin_log.cpp
                               ../../plugin_api/data_chunk.h)
# Make a separate lib for plugins.
