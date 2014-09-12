project(mod_server)

if (WIN32)
  set(PLATFORM_DEP_SOURCES socket_win.cpp)
else()
  set(PLATFORM_DEP_SOURCES "")
endif()

if (VERBOSE)
  message(STATUS "PLATFORM_DEP_SOURCES: ${PLATFORM_DEP_SOURCES}")
endif()

add_library(socket socket.cpp socket.h
                   tcp_listener.cpp tcp_listener.h
                   tcp_socket.cpp tcp_socket.h
                   ${PLATFORM_DEP_SOURCES})
