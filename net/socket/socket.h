#ifndef NET_SOCKET_SOCKET_H_
#define NET_SOCKET_SOCKET_H_

#include "base/build_required.h"

#if defined(UNIX)
#include <sys/socket.h>
#endif

namespace net {
#if defined(UNIX)
using SOCK_TYPE = int;
SOCK_TYPE kInvalidSocket = -1;
#endif

class Socket {
 public:
  DISALLOW_COPY_AND_ASSIGN(Socket);
  DISALLOW_MOVE(Socket);
  Socket() = default;
  virtual ~Socket();

  // TODO(Olster): Rethink this function.
  static int Select(int maxFd, fd_set* readSet,
                      fd_set* writeSet, fd_set* errSet,
                      timeval* timeout);

  virtual bool Open() = 0;
  bool Close();

  enum ShutDownCode {
#if defined(UNIX)
    READ = SHUT_RD,
    WRITE = SHUT_WR,
    BOTH = SHUT_RDWR
#endif
  };

  bool ShutDown(ShutDownCode code = BOTH);

  SOCK_TYPE handle() const { return m_socket; }
 protected:
  bool OpenHelper(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

  SOCK_TYPE m_socket = kInvalidSocket;
};
} // namespace net
#endif // NET_SOCKET_SOCKET_H_
