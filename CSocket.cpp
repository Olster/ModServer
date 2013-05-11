#include "CSocket.h"

#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

CSocket::CSocket(SOCK_DOMAIN domain, SOCK_TYPE type, SOCK_PROTOCOL protocol) {
  int nProtocol;
  switch (protocol) {
    case TCP:
      nProtocol = getprotobyname("tcp")->p_proto;
    break;

    case UDP:
      nProtocol = getprotobyname("udp")->p_proto;
    break;

    default:
    break;
  }

  m_socket = socket(domain, type, nProtocol);

  // Unblocking the blocked socket
  int yes = 1;
  setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  // socket() returns -1 on error
  m_bReady = m_socket != -1;
}

CSocket::CSocket(int sock_fd) {
  m_socket = sock_fd;
  m_bReady = true;
}

CSocket::~CSocket() {
  close(m_socket);
}

int CSocket::Send(const std::string& data) {
  return send(m_socket, data.c_str(), data.length(), 0);
}

int CSocket::Receive(std::string& out) {
  char str[1024];
  int read = recv(m_socket, str, (sizeof str)/(sizeof str[0]), 0);

  if (read < 1) {
    return read;
  }

  str[read - 2] = '\0';

  out = str;

  return read;
}

bool CSocket::Close(CLOSE_TYPE type) {
  int errorCode = shutdown(m_socket, type);

  // if errorCode is 0, then the call was successful
  return errorCode == 0;
}

int CSocket::Select(int maxFd, fd_set* readList,
                    fd_set* writeList, fd_set* errorList,
                    timeval* tv) {

  return select(maxFd, readList, writeList, errorList, tv);
}
