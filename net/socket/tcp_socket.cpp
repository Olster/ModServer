#include "net/socket/tcp_socket.h"

TcpSocket::TcpSocket(SOCK_TYPE sockFd) {
  m_socket = sockFd;
}

TcpSocket::~TcpSocket() {}

bool TcpSocket::Open() {
  return OpenHelper(AF_INET, SOCK_STREAM, 0);
}

int TcpSocket::Send(const char* data, int sizeBytes) {
  return send(m_socket, data, sizeBytes, 0);
}

int TcpSocket::Receive(char* data, int sizeBytes) {
  return recv(m_socket, data, sizeBytes, 0);
}
