#include "net/socket/tcp_socket.h"

namespace net {
bool TcpSocket::Open() {
  // Use default parameters: they default to TCP socket.
  return OpenHelper();
}
} // namespace net
