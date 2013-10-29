#include "net/socket/socket.h"

#include <assert.h>

class NetworkIniter {
 public:
  NetworkIniter() {
    WSADATA data = {0};
    bool init = WSAStartup(MAKEWORD(2, 2), &data) == 0;
    assert(init && "Socket lib has not been inited");
  }

  ~NetworkIniter() {
    WSACleanup();
  }
};

// This object initializes and uninitializes networking library in windows.
NetworkIniter g_networkiniter;

namespace net {
Socket::~Socket() {
  ShutDown();
  Close();
}

bool Socket::Close() {
  return closesocket(m_socket) == 0;
}

bool Socket::ShutDown(ShutDownCode code) {
  return shutdown(m_socket, code) == 0;
}
} // namespace net
