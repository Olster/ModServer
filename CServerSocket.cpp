#include "CServerSocket.h"
#include <iostream>
#include <netinet/in.h>

CServerSocket::CServerSocket(unsigned int port) {
  m_bReady = SocketIsReady();
  m_dPort = port;
  if (m_bReady) {
    sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(m_dPort);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    // TODO(Olster): Don't do bind() in a constructor, it might fail
    int retval = bind(m_socket, (sockaddr*)&myaddr, sizeof myaddr);
    if (retval < 0) {
      // Failed to bind a socket
      m_bReady = false;
    }
  }

  if (m_bReady) {
#ifdef DEBUG
    std::cout << "Socket created: " << m_socket << std::endl;
  }
#endif
}

bool CServerSocket::Listen(int pendingConnections) {
  m_bReady = listen(m_socket, pendingConnections) == 0;
#ifdef DEBUG
  std::cout << "Socket is listening on port " << m_dPort << std::endl;
#endif
}

int CServerSocket::Accept() {
  socklen_t len;
  int socket = accept(m_socket, NULL, &len);

#ifdef DEBUG
  std::cout << "Socket accepted connection: " << socket << std::endl;
#endif

  return socket;
}
