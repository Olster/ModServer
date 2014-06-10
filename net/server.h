#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include <map>
#include <string>
#include <memory>

#include "base/build_required.h"
#include "net/socket/tcp_listener.h"

class IPEndPoint;
class ProtocolFactory;
class ProtocolHandler;

class Server {
 public:
  DISALLOW_COPY_AND_ASSIGN(Server);
  DISALLOW_MOVE(Server);
  
  Server(const IPEndPoint& endPoint,
         ProtocolFactory* protFactory);
  ~Server();

  enum StartErrorCode {
    SUCCESS = 0,
    SOCKET_NOT_OPENED,
    SOCKET_NOT_BOUND,
    SOCKET_NOT_LISTENING
  };
  
  StartErrorCode Start();

  enum UpdateCode {
    TIME_OUT,
    UPDATE_ERROR,
    OK
  };

  // Updates info on connections: whether ready to read, write, in error state.
  UpdateCode UpdateConnections();
  
  // Accepts pending connections.
  void AcceptNewConnections();
  
  // Reads data from all sockets that are ready to read and passes it
  // to the protocol handler.
  void ReadRequests();

  // Sends data to all sockets that are ready to write if data is available.
  // Data produced by protocol handler.
  void SendResponses();

  // TODO(Olster): Implement stop functionality.
  bool Stopped() { return false; }

  // TODO(Olster): Implement shutdown functionality.
  void Shutdown() {}
 private:
  TcpListener m_listener;

  std::unique_ptr<ProtocolFactory> m_protoFactory;

  std::map<TcpSocket*, ProtocolHandler*> m_sessions;
  
  // NOTE(Olster): There's no point of setting max listen to more than
  // we can handle simultaneously in select().
  int m_maxListen = FD_SETSIZE - 2;

  unsigned int m_maxFd = 0;

  fd_set m_masterSet;
  fd_set m_readSet;
  fd_set m_writeSet;
  fd_set m_errorSet;

  timeval m_timeout;
};

#endif // NET_HTTP_SERVER_H_
