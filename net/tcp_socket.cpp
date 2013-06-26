#include "net/tcp_socket.h"

#include <netinet/in.h>
#include <sys/socket.h>

namespace net {

TCPSocket::TCPSocket(Socket::SOCK_DOMAIN domain)
 : Socket(domain) {

}

bool TCPSocket::Open() {
  m_socket = socket(m_domain, SOCK_STREAM, IPPROTO_TCP);

  // Unblocking the blocked socket
  int yes = 1;
  setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  // socket() returns -1 on error
  m_bReady = m_socket != -1;

  return m_bReady;
}

int TCPSocket::Send(const std::string& data) {
  return send(m_socket, data.c_str(), data.length(), 0);
}

int TCPSocket::Receive(std::string& data) {
  char str[1024*4]; // 4kb
  int read = recv(m_socket, str, (sizeof str)/(sizeof str[0]), 0);

  if (read < 1) {
    return read;
  }

  str[read - 2] = '\0';

  data = str;

  return read;
}

} // namespace net
