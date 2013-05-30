#ifndef NET_TCPSOCKET_H_
#define NET_TCPSOCKET_H_

#include "net/Socket.h"

namespace net {

class TCPSocket : public Socket {
 public:
  TCPSocket(Socket::SOCK_DOMAIN domain);
  virtual ~TCPSocket() {}

  void operator=(const TCPSocket& other) = delete;
  TCPSocket(const TCPSocket& other) = delete;
  TCPSocket(const TCPSocket&& other) = delete;

  int Open() override;

  int Send(const std::string& data);

  int Receive(std::string& data);

  std::string ToString() const override { return Socket::ToString() + "TCPSocket."; }
};

} // namespace net
#endif // NET_TCPSOCKET_H_
