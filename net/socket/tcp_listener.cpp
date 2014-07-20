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

bool TcpListener::Bind(int* err) {
  if (!m_ep.IsValid()) {

    if (err) {
      *err = EINVAL;
    }

    return false;
  }

  sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_ep.port());
  addr.sin_addr.s_addr = inet_addr(m_ep.ip());

  int rc = bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

  if (err) {
    *err = SocketError();
  }

  return rc == 0;
}

bool TcpListener::Listen(int maxListeners, int* err) {
  int rc = listen(m_socket, maxListeners);

  if (err) {
    *err = SocketError();
  }

  return rc == 0;
}

Socket::SOCK_TYPE TcpListener::Accept(int* err) {
  SOCK_TYPE acceptedSock = accept(m_socket, NULL, NULL);

  if (err) {
    *err = SocketError();
  }

  return acceptedSock;
}
