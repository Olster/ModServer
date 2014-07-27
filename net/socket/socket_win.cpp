#include "net/socket/socket.h"

#include <cassert>
#include <cstdio>

class NetworkIniter {
 public:
  NetworkIniter() {
    WSADATA data = {0};
    bool init = ::WSAStartup(MAKEWORD(2, 2), &data) == 0;
    assert(init && "Socket lib has not been inited");

    if (!init) {
      fprintf(stderr, "Windows socket lib wasn't init\n");
    }
  }

  ~NetworkIniter() {
    ::WSACleanup();
  }
};

// This object initializes and uninitializes networking library in windows.
NetworkIniter g_networkiniter;
