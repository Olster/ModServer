#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <sys/socket.h>

// TODO(Olster) add support for UDP. Even make them separate

class CSocket {
 public:
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
  CSocket(int sock_fd);
  virtual ~CSocket();

  void operator=(const CSocket& other) = delete;
  CSocket(const CSocket& other) = delete;

  bool SocketIsReady() const { return m_bReady; }
  int& GetHandle() { return m_socket; }

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
  int m_socket;

 private:
  int m_nLastError = 0;
  bool m_bReady = false;;
};

#endif // CSOCKET_H_
