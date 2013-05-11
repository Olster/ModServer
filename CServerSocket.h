#ifndef CSERVERSOCKET_H_
#define CSERVERSOCKET_H_

#include "CSocket.h"

namespace net {

class CServerSocket : public CSocket {
 public:
  CServerSocket(unsigned int port = 2563);
  ~CServerSocket() = default;

  bool Bind();

  bool Listen(int pendingConnections);

  // Returns accepted socket type that can be passed into CSocket ctor
  SOCK_T Accept();

  bool ServerIsReady() const { return m_bReady; }

 private:
  bool m_bReady;
  unsigned int m_dPort;
};

} // namespace net
#endif // CSERVERSOCKET_H_
