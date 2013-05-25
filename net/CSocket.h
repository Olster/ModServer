#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <sys/socket.h>
#include <string>

namespace net {
// TODO(Olster) add support for UDP. Even make them separate
#ifdef WIN32
typedef SOCKET SOCK_T;
const SOCK_T kInvalidSocket = INVALID_SOCKET;
#elif defined(UNIX)
typedef int SOCK_T;
const SOCK_T kInvalidSocket = -1;
#endif

class CSocket {
 public:
 // NOTE(Olster): It's C++11, so I could use class enum
 // but in this circumastances I need to use these as ints as well

  enum SOCK_DOMAIN {
    IPv4 = PF_INET,
    IPv6 = PF_INET6
  };

  enum SOCK_TYPE {
    STREAM = SOCK_STREAM,
    DATAGRAM = SOCK_DGRAM,
    RAW = SOCK_RAW
  };

  enum SOCK_PROTOCOL {
    TCP,
    UDP
  };

  CSocket(SOCK_DOMAIN domain = IPv4, SOCK_TYPE type = STREAM, SOCK_PROTOCOL protocol = TCP);
  CSocket(SOCK_T sock_fd);
  virtual ~CSocket();

  void operator=(const CSocket& other) = delete;
  CSocket(const CSocket& other) = delete;

  bool SocketIsReady() const { return m_bReady; }
  SOCK_T& GetHandle() { return m_socket; }

  int Receive(std::string& out);

  // Returns number of chars (bytes?) sent
  int Send(const std::string& data);

  enum CLOSE_TYPE {
    CLOSE_RECEIVE = 0,
    CLOSE_SEND,
    CLOSE_ALL
  };

  bool Close(CLOSE_TYPE type = CLOSE_ALL);

  // TODO(Olster): Full rewrite of Select()
  static int Select(int maxFd, fd_set* readList, fd_set* writeList,
                     fd_set* errorList, timeval* tv);
 protected:
  SOCK_T m_socket;

 private:
  int m_nLastError = 0;
  bool m_bReady = false;;
};

} // namespace net

#endif // CSOCKET_H_