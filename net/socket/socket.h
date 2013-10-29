#ifndef NET_SOCKET_SOCKET_H_
#define NET_SOCKET_SOCKET_H_

#include "base/build_required.h"

#if defined(UNIX)
#include <sys/socket.h>
#elif defined(WIN32)
#include <winsock2.h>
#endif

namespace net {
#if defined(UNIX)
using SOCK_TYPE = int;
#elif defined(WIN32)
using SOCK_TYPE = SOCKET;
#endif

class Socket {
 public:
#if defined(UNIX)
  SOCK_TYPE kInvalidSocket = -1;
#elif defined(WIN32)
  SOCK_TYPE kInvalidSocket = INVALID_SOCKET;
#endif

  DISALLOW_COPY_AND_ASSIGN(Socket);
  DISALLOW_MOVE(Socket);
  Socket() = default;
  virtual ~Socket();

  // TODO(Olster): Rethink this function.
  static int Select(int maxFd, fd_set* readSet,
                    fd_set* writeSet, fd_set* errSet,
                    timeval* timeout) {
    return select(maxFd, readSet, writeSet, errSet, timeout);
  }

  virtual bool Open() = 0;
  bool Close();

  enum ShutDownCode {
#if defined(UNIX)
    READ = SHUT_RD,
    WRITE = SHUT_WR,
    BOTH = SHUT_RDWR
#elif defined(WIN32)
    READ = SD_RECEIVE,
    WRITE = SD_SEND,
    BOTH = SD_BOTH
#endif
  };

  bool ShutDown(ShutDownCode code = BOTH);

  SOCK_TYPE handle() const { return m_socket; }
 protected:
  bool OpenHelper(int domain, int type, int protocol) {
    m_socket = socket(domain, type, protocol);
    return m_socket != kInvalidSocket;
  }

  SOCK_TYPE m_socket = kInvalidSocket;
};
} // namespace net
#endif // NET_SOCKET_SOCKET_H_
