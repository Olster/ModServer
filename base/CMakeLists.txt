project(mod_server)

# TODO(Olster): Simplify this.
if (WIN32)
  set(PLATFORM_DEP_SOURCES dynamic_lib_win.cpp os_info_win.cpp path_win.cpp)
else()
  set(PLATFORM_DEP_SOURCES dynamic_lib_unix.cpp os_info_unix.cpp path_unix.cpp)
endif()

add_library(base command_line.cpp command_line.h
                 dynamic_lib.h
                 encoding.cpp encoding.h
                 os_info.h
                 path.h
                 logger.cpp logger.h ${PLATFORM_DEP_SOURCES})
