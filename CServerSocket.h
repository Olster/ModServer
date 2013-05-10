#ifndef CSERVERSOCKET_H_
#define CSERVERSOCKET_H_

#include "CSocket.h"
#include <string>

class CServerSocket : public CSocket {
 public:
  CServerSocket(unsigned int port = 2563);
  ~CServerSocket() = default;

  bool Listen(int pendingConnections);

  // TODO(Olster): For portability, make a Networking namespace and
  // typedef socket type

  // Returns accepted socket. Return will be changed to typedef'ed
  // socket type
  int Accept();

  // NOTE: Temporarily added |sock|
  int Receive(int scok, std::string& out);

  // Returns number of chars (bytes?) sent
  int Send(int sock, const std::string& data);

  bool ServerIsReady() const { return m_bReady; }

 private:
  bool m_bReady;
  unsigned int m_dPort;
};

#endif // CSERVERSOCKET_H_
