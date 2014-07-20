#include "net/socket/tcp_socket.h"

TcpSocket::TcpSocket(SOCK_TYPE sockFd) {
  m_socket = sockFd;
}

TcpSocket::~TcpSocket() {}

bool TcpSocket::Open(int* err) {
  return OpenHelper(AF_INET, SOCK_STREAM, 0, err);
}

int TcpSocket::Send(const char* data, int sizeBytes, int* err) {
  int rc = send(m_socket, data, sizeBytes, 0);

  if (err) {
    *err = SocketError();
  }

  return rc;
}

int TcpSocket::Receive(char* data, int sizeBytes, int* err) {
  int rc = recv(m_socket, data, sizeBytes, 0);

  if (err) {
    *err = SocketError();
  }

  return rc;
}
