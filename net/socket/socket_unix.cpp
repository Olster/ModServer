#include "net/socket/socket.h"

#include <unistd.h>

namespace net {
Socket::~Socket() {
  ShutDown();
  Close();
}

bool Socket::Close() {
  return close(m_socket);
}

bool Socket::ShutDown(ShutDownCode code) {
  return shutdown(m_socket, code) != 0;
}

bool Socket::OpenHelper(int domain, int type, int protocol) {
  return socket(domain, type, protocol) != kInvalidSocket;
}
} // namespace net
