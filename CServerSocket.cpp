#include "CServerSocket.h"
#include <iostream>
#include <netinet/in.h>

namespace net {

CServerSocket::CServerSocket(unsigned int port) {
  m_bReady = SocketIsReady();
  m_dPort = port;

  if (m_bReady) {
#ifdef DEBUG
    std::cout << "Socket created, redy to be bound: " << m_socket << std::endl;
  }
#endif
}

bool CServerSocket::Bind() {
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

bool CServerSocket::Listen(int pendingConnections) {
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

SOCK_T CServerSocket::Accept() {
  socklen_t len;
  int socket = accept(m_socket, NULL, &len);

#ifdef DEBUG
  std::cout << "Socket accepted connection: " << socket << std::endl;
#endif

  return socket;
}

} // namespace net
