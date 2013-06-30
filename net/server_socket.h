#ifndef NET_SERVERSOCKET_H_
#define NET_SERVERSOCKET_H_

#include "net/tcp_socket.h"

namespace net {

class ServerSocket : public TCPSocket {
 public:
  ServerSocket(unsigned int port, SOCK_DOMAIN domain = SOCK_DOMAIN::IPv4);
  ~ServerSocket() = default;

  void operator=(const ServerSocket& other) = delete;
  ServerSocket(const ServerSocket& other) = delete;
  void operator=(ServerSocket&& other) = delete;
  ServerSocket(ServerSocket&& other) = delete;

  bool Bind();

  bool Listen(int pendingConnections);

  TCPSocket* Accept();

  bool IsServerSockReady() const { return m_bReady; }

  int GetPort() const { return m_dPort; }
  int GetPendingConnections() const { return m_dPendingConnections; }

  std::string ToString() const override { return TCPSocket::ToString() + "ServerSocket."; }
 private:
  int m_dPort;
  int m_dPendingConnections;
};

} // namespace net

#endif // CSERVERSOCKET_H_
