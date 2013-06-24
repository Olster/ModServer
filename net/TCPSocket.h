#ifndef NET_TCPSOCKET_H_
#define NET_TCPSOCKET_H_

#include "net/Socket.h"

namespace net {

class TCPSocket : public Socket {
 public:
  TCPSocket(Socket::SOCK_DOMAIN domain);
  ~TCPSocket() {}

  void operator=(const TCPSocket& other) = delete;
  TCPSocket(const TCPSocket& other) = delete;
  void operator=(TCPSocket&& other) = delete;
  TCPSocket(TCPSocket&& other) = delete;

  bool Open() override;

  int Send(const std::string& data);

  int Receive(std::string& data);

  std::string ToString() const override { return Socket::ToString() + "TCPSocket."; }
 private:
  // TODO(Olster): Investigate another way of letting ServerSocket
  // access protected members of TCPSocket
  friend class ServerSocket;
};

} // namespace net

#endif // NET_TCPSOCKET_H_
