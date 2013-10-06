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

  assert(strlen(ip) < sizeof(m_ip));
  strcpy(m_ip, ip);
}

bool ServerSocket::Bind() {
  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_port);
  addr.sin_addr.s_addr = inet_addr(m_ip);

  return bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

} // namespace net
