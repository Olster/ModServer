#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include <sys/socket.h>

#include "base/Object.h"

namespace net {

#if defined(WIN32)
using InternalSockType = SOCKET;
const InternalSockType kInvalidSocket = INVALID_SOCKET;
#elif defined(UNIX)
using InternalSockType = int;
const InternalSockType kInvalidSocket = -1;
#endif

class Socket : public base::Object {
 public:
  // NOTE(Olster): It's C++11, so I could use class enum
  // but in these circumastances I need to use these as ints as well
  enum SOCK_DOMAIN {
    IPv4 = PF_INET,
    IPv6 = PF_INET6
  };

  Socket(SOCK_DOMAIN domain);
  virtual ~Socket();

  void operator=(const Socket& other) = delete;
  Socket(const Socket& other) = delete;
  Socket(const Socket&& other) = delete;

  bool SocketIsReady() const { return m_bReady; }

  InternalSockType GetHandle() const { return m_socket; }

  virtual int Open() = 0;

  enum CLOSE_TYPE {
    CLOSE_RECEIVE = 0,
    CLOSE_SEND,
    CLOSE_ALL
  };

  bool Close(CLOSE_TYPE type = CLOSE_ALL);

  // TODO(Olster): Full rewrite of Select()
  static int Select(int maxFd, fd_set* readList, fd_set* writeList,
                    fd_set* errorList, timeval* tv);

  virtual std::string ToString() const override { return "net::Socket."; }
 protected:
  // Sets new handle for the socket, sets |m_bReady| flag to false
  void SetHandle(InternalSockType sock);

  // NOTE(Olster): Only derived classes can set socket to ready state
  bool SocketSetReady(bool isReady) { m_bReady = isReady; }

  SOCK_DOMAIN m_domain;

  InternalSockType m_socket = kInvalidSocket;
  bool m_bReady = false;
};

} // namespace net

#endif // net_Socket_H_
