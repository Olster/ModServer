#ifndef NET_HTTPSERVER_H_
#define NET_HTTPSERVER_H_

#include <list>

#include "base/object.h"
#include "base/build_required.h"

#include "net/server_socket.h"
#include "net/tcp_session.h"

namespace net {

class HTTPServer : public base::Object {
 public:
  // |filePath| is where the server will look for resources that client
  // requests
  HTTPServer(int port, int pendingConnections, std::string filePath);
  ~HTTPServer();

  DISALLOW_COPY_AND_ASSIGN(HTTPServer);
  DISALLOW_MOVE(HTTPServer);

  // Calls select() on each session socket, modyfying sets
  // of file descriptors.
  // Returns false if select() failed, fills the m_strErrMsg with
  // error message, sets m_bIsInErrorState flag to true
  bool UpdateSessions();
  void CloseTimedOutSessions();
  void AcceptNewConnections();

  // Receiving and sending data
  void ProcessSessions();

  bool IsInErrorState() const { return m_bIsInErrorState; }
  std::string GetErrorMsg() { return m_strErrMsg; }

  std::string ToString() const override { return "net::HTTPServer"; }
 private:
  net::ServerSocket m_serverSock;
  std::string m_resourcePath;
  std::list<net::TCPSession*> m_sessions;
  std::list<net::TCPSession*> m_removeList;

  fd_set m_masterSet;
  fd_set m_readSet;
  fd_set m_writeSet;

  int m_dMaxFileDescr = 0;

  // Select() timeout
  timeval m_timeout;

  // Server is in error state initially since it's not set up properly
  bool m_bIsInErrorState = true;
  std::string m_strErrMsg = "";
};

} // namespace net

#endif // NET_HTTPSERVER_H_
