#ifndef NET_HTTP_SERVER_H_
#define NET_HTTP_SERVER_H_

#include <list>
#include <string>

#include "base/build_required.h"
#include "net/socket/server_socket.h"

namespace net {
class HttpConnection;

class HttpServer {
 public:
  DISALLOW_COPY_AND_ASSIGN(HttpServer);
  DISALLOW_MOVE(HttpServer);
  
<<<<<<< HEAD
  HttpServer(const char* ip, unsigned short port,
              const std::string& resFolder, int maxListen = 10);
=======
  HttpServer(const char* ip, unsigned short port, const std::string& resFolder);
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
  ~HttpServer();

  enum class StartErrorCode : unsigned char {
    SUCCESS = 0,
    SOCKET_NOT_OPENED,
<<<<<<< HEAD
    SOCKET_NOT_BOUND,
    SOCKET_NOT_LISTENING
=======
    SOCKET_NOT_BOUND
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
  };
  
  StartErrorCode Start();  
  
<<<<<<< HEAD
  std::string ErrorString() const { return m_errString; }
=======
  std::string ErrorString() const;
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d

  // Calls select() on all connected sockets and the listening socket.
  int UpdateConnections();
  
  // Accepts pending connections.
  void AcceptNewConnections();
  
  // Closes connections that have been server and disconnected/asked to close
<<<<<<< HEAD
  // the connection; or closing those connections that are in error state.
=======
  // the connection; or closing those connecations that are in error state.
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
  void CloseUntrackedConnections();
  
  void ReadRequests();
  void ProcessRequests();
  void SendResponses();
 private:
  ServerSocket m_listenerSocket;
  std::string m_resourcesFolderPath;  
  std::list<HttpConnection*> m_connections;
<<<<<<< HEAD
  std::string m_errString;
  int m_maxListen;

  int m_maxFd = 0;

  fd_set m_masterSet;
  fd_set m_readerSet;
  fd_set m_writerSet;
  fd_set m_errorSet;

  timeval m_timeout;
=======
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
};
} // namespace net

#endif // NET_HTTP_SERVER_H_
