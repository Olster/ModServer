#include "ServerSocket.h"
#include <iostream>
#include <netinet/in.h>

namespace net {

ServerSocket::ServerSocket(unsigned int port, SOCK_DOMAIN domain)
 : TCPSocket(domain) {
  m_dPort = port;

  // m_bReady is false, guaranteed by initialization in Socket class
}

bool ServerSocket::Bind() {
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
    std::cout << "Socket hasn't been opened" << std::endl;
#endif
    return m_bReady;
  }

#ifdef DEBUG
  if (!m_bReady) {
    std::cout << "Couldn't bind the socket" << std::endl;
  } else {
    std::cout << "Socket is bound" << std::endl;
  }
#endif

  return m_bReady;
}

bool ServerSocket::Listen(int pendingConnections) {
  m_bReady = listen(m_socket, pendingConnections) == 0;
#ifdef DEBUG
  if (!m_bReady) {
    std::cout << "Listen() failed" << std::endl;
  } else {
    std::cout << "Socket is listening on port " << m_dPort << std::endl;
  }
#endif

  return m_bReady;
}

TCPSocket* ServerSocket::Accept() {
  socklen_t sockLen;
  SOCK_T sock = accept(m_socket, 0, &sockLen);

  // accept() returns negative file descr on error
  if (sock < 0) {
    return nullptr;
  }

  TCPSocket* retSock = new TCPSocket(m_domain);
  retSock->GetHandle() = sock;
  retSock->SocketSetReady(true);
}

} // namespace net
