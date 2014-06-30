#include "net/socket/tcp_listener.h"

#include <assert.h>
#include <string.h>

#if defined(UNIX)
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

TcpListener::TcpListener(const IPEndPoint& ep)
 : m_ep(ep.ip(), ep.port()) {}

TcpListener::TcpListener(const char* ip, unsigned short port)
 : m_ep(ip, port) {}

bool TcpListener::Bind() {
  if (!m_ep.IsValid()) {
    return false;
  }

  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_ep.port());
  addr.sin_addr.s_addr = inet_addr(m_ep.ip());

  return bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

bool TcpListener::Listen(int maxListeners) {
  return listen(m_socket, maxListeners) == 0;
}

Socket::SOCK_TYPE TcpListener::Accept() {
  return accept(m_socket, nullptr, nullptr);
}
