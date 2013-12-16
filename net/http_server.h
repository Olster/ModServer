#ifndef NET_HTTP_SERVER_H_
#define NET_HTTP_SERVER_H_

#include <list>
#include <string>

#include "base/build_required.h"
#include "net/socket/server_socket.h"

class HttpConnection;

class HttpServer {
 public:
  DISALLOW_COPY_AND_ASSIGN(HttpServer);
  DISALLOW_MOVE(HttpServer);
  
  HttpServer(const char* ip, unsigned short port,
             const std::string& resFolder, int maxListen = 10);
  ~HttpServer();

  enum StartErrorCode {
    SUCCESS = 0,
    SOCKET_NOT_OPENED,
    SOCKET_NOT_BOUND,
    SOCKET_NOT_LISTENING
  };
  
  StartErrorCode Start();  
  std::string ErrorString() const { return m_errString; }

  enum UpdateCode {
    TIME_OUT,
    UPDATE_ERROR,
    OK
  };

  // Updates info on connections: whether ready to read, write, in error state.
  UpdateCode UpdateConnections();
  
  // Accepts pending connections.
  void AcceptNewConnections();
  
  // Closes connections that have been server and disconnected/asked to close
  // the connection; or closing those connections that are in error state.
  void CloseUntrackedConnections();
  
  void ReadRequests();
  void ProcessRequests();
  void SendResponses();
 private:
  ServerSocket m_listenerSocket;
  std::string m_resourcesFolderPath;  
  std::list<HttpConnection*> m_connections;
  std::string m_errString;
  int m_maxListen;

  unsigned int m_maxFd = 0;

  fd_set m_masterSet;
  fd_set m_readSet;
  fd_set m_writeSet;
  fd_set m_errorSet;

  timeval m_timeout;
};

#endif // NET_HTTP_SERVER_H_
