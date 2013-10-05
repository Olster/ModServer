#ifndef NET_SOCKET_SERVER_SOCKET_H_
#define NET_SOCKET_SERVER_SOCKET_H_

#include "net/socket/tcp_socket.h"

namespace net {
class ServerSocket : public TcpSocket {
public:
  DISALLOW_COPY_AND_ASSIGN(ServerSocket);
  DISALLOW_MOVE(ServerSocket);

  ServerSocket(const char* ip, unsigned short port);
  ~ServerSocket();

private:
  char* m_ip = nullptr;
  unsigned short m_port = 0;
};
} // namespace net

#endif // NET_SOCKET_SERVER_SOCKET_H_
