#include "net/server_socket.h"
#include <iostream>
#include <netinet/in.h>

namespace net {

ServerSocket::ServerSocket(unsigned int port, SOCK_DOMAIN domain)
 : TCPSocket(domain), m_dPort(port) {
#ifdef DEBUG
  std::cout << "Info: Creating server socket on port " << m_dPort << std::endl;
#endif

  // m_bReady is false, guaranteed by initialization in Socket class
}

bool ServerSocket::Bind() {
  // |m_bReady| is set to |true| by successful Open()
  if (m_bReady) {
    sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(m_dPort);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    int retval = bind(m_socket, (sockaddr*)&myaddr, sizeof myaddr);
    if (retval < 0) {
      // Failed to bind a socket
      m_bReady = false;
    }
  } else {
#ifdef DEBUG
    std::cout << "Error: Socket hasn't been opened" << std::endl;
#endif
    return false;
  }

#ifdef DEBUG
  if (!m_bReady) {
    std::cout << "Error: Couldn't bind the socket" << std::endl;
  } else {
    std::cout << "Info: Socket is bound" << std::endl;
  }
#endif

  return m_bReady;
}

bool ServerSocket::Listen(int pendingConnections) {
  if (pendingConnections < 1) {
    return false;
  }

  if (m_bReady) {
    m_bReady = listen(m_socket, pendingConnections) == 0;
  }

#ifdef DEBUG
  if (!m_bReady) {
    std::cout << "Error: Listen() failed" << std::endl;
  } else {
    std::cout << "Info: Socket is listening on port " << m_dPort << std::endl;
  }
#endif

  return m_bReady;
}

TCPSocket* ServerSocket::Accept() {
  if (!m_bReady) {

#ifdef DEBUG
    std::cout << "Error: Socket is not ready to accept" << std::endl;
#endif

    return nullptr;
  }

  socklen_t sockLen;
  InternalSockType sock = accept(m_socket, 0, &sockLen);

  // accept() returns negative file descr on error
  if (sock < 0) {
    return nullptr;
  }

  TCPSocket* retSock = new TCPSocket(m_domain);
  retSock->m_socket = sock;
  retSock->m_bReady = true;

  return retSock;
}

} // namespace net
