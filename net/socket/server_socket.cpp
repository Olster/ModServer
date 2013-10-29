#include "net/socket/server_socket.h"

#include <assert.h>
#include <string.h>

#if defined(UNIX)
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace net {
ServerSocket::ServerSocket(const char* ip, unsigned short port)
 : m_port(port) {
  assert(ip && "IP cannot be null");

  // Just clear the string.
  memset(m_ip, 0, sizeof(m_ip));

  assert(strlen(ip) < sizeof(m_ip));
  strcpy_s(m_ip, ip);
}

bool ServerSocket::Bind() {
  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_port);
  addr.sin_addr.s_addr = inet_addr(m_ip);

  return bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

bool ServerSocket::Listen(int maxListeners) {
  return listen(m_socket, maxListeners) == 0;
}

TcpSocket* ServerSocket::Accept() {
  SOCK_TYPE sock = accept(m_socket, nullptr, nullptr);
  return (sock != kInvalidSocket) ? new TcpSocket(sock) : nullptr;
}
} // namespace net
