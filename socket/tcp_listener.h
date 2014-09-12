#ifndef NET_SOCKET_TCP_LISTENER_H_
#define NET_SOCKET_TCP_LISTENER_H_

#include "server_core/ip_endpoint.h"
#include "socket/tcp_socket.h"

class TcpListener : public TcpSocket {
 public:
  explicit TcpListener(const IPEndPoint& ep);
  TcpListener(const char* ip, unsigned short port);
  ~TcpListener() {}

  bool Bind(int* err = NULL);
  bool Listen(int maxListeners, int* err = NULL);

  SOCK_TYPE Accept(int* err = NULL);
 private:
  IPEndPoint m_ep;
};

#endif  // NET_SOCKET_TCP_LISTENER_H_
