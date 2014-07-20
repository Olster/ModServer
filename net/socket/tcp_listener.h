#ifndef NET_SOCKET_SERVER_SOCKET_H_
#define NET_SOCKET_SERVER_SOCKET_H_

#include "net/socket/tcp_socket.h"
#include "net/ip_endpoint.h"

class TcpListener : public TcpSocket {
 public:
  TcpListener(const IPEndPoint& ep);
  TcpListener(const char* ip, unsigned short port);
  ~TcpListener() {}

  bool Bind(int* err = NULL);
  bool Listen(int maxListeners, int* err = NULL);

  SOCK_TYPE Accept(int* err = NULL);
 private:
  IPEndPoint m_ep;
};

#endif // NET_SOCKET_SERVER_SOCKET_H_
