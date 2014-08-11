#ifndef NET_SOCKET_SOCKET_H_
#define NET_SOCKET_SOCKET_H_

#include "base/build_required.h"

#if defined(UNIX)
#include <errno.h>
#include <sys/socket.h>
#elif defined(WIN32)
#include <winsock2.h>
#endif

class Socket {
 public:
#if defined(UNIX)
  typedef int SOCK_TYPE;
  static const SOCK_TYPE kInvalidSocket;
#elif defined(WIN32)
  typedef SOCKET SOCK_TYPE;
  static const SOCK_TYPE kInvalidSocket;
#endif

  Socket()
   : m_socket(kInvalidSocket) {}

  virtual ~Socket();

  // TODO(Olster): Rethink this function.
  static int Select(int maxFd, fd_set* readSet,
                    fd_set* writeSet, fd_set* errSet,
                    timeval* timeout) {
    return select(maxFd, readSet, writeSet, errSet, timeout);
  }

  virtual bool Open(int* err = NULL) = 0;
  bool Close(int* err = NULL);

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

  bool ShutDown(ShutDownCode code = BOTH, int* err = NULL);

  SOCK_TYPE handle() const { return m_socket; }
 protected:
  bool OpenHelper(int domain, int type, int protocol, int* err = NULL);

  int SocketError() {
#if defined(WIN32)
    return ::WSAGetLastError();
#else
    return errno;
#endif
  }

  SOCK_TYPE m_socket;

 private:
  DISALLOW_COPY_AND_ASSIGN(Socket);
  DISALLOW_MOVE(Socket);
};
#endif // NET_SOCKET_SOCKET_H_
