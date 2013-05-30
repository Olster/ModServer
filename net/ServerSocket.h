#ifndef NET_SERVERSOCKET_H_
#define NET_SERVERSOCKET_H_

#include "TCPSocket.h"

namespace net {

class ServerSocket : public TCPSocket {
 public:
  ServerSocket(unsigned int port, SOCK_DOMAIN domain = SOCK_DOMAIN::IPv4);
  ~ServerSocket() = default;

  bool Bind();

  bool Listen(int pendingConnections);

  TCPSocket* Accept();

  bool ServerIsReady() const { return m_bReady; }

  std::string ToString() const override { return TCPSocket::ToString() + "ServerSocket"; }
 private:
  unsigned int m_dPort;
};

} // namespace net
#endif // CSERVERSOCKET_H_
