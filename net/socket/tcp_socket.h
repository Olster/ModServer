#ifndef NET_SOCKET_TCP_SOCKET_H_
#define NET_SOCKET_TCP_SOCKET_H_

#include "net/socket/socket.h"

namespace net {
class TcpSocket : public Socket {
 public:
  DISALLOW_COPY_AND_ASSIGN(TcpSocket);
  DISALLOW_MOVE(TcpSocket);

  TcpSocket() = default;
  ~TcpSocket() = default;

  bool Open() override;
};
} // namespace net
#endif // NET_SOCKET_TCP_SOCKET_H_
