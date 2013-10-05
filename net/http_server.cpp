#include "net/http_server.h"

namespace net {
HttpServer::HttpServer(const char* ip, unsigned short port,
                          const std::string& resFolder, int maxListen)
 : m_listenerSocket(ip, port),
   m_resourcesFolderPath(resFolder),
   m_maxListen(maxListen) {}

HttpServer::~HttpServer() {
  m_listenerSocket.ShutDown();
  m_listenerSocket.Close();
}

HttpServer::StartErrorCode HttpServer::Start() {
  if (!m_listenerSocket.Open()) {
    return StartErrorCode::SOCKET_NOT_OPENED;
  }

  if (!m_listenerSocket.Bind()) {
    return StartErrorCode::SOCKET_NOT_BOUND;
  }

  if (!m_listenerSocket.Listen(m_maxListen)) {
    return StartErrorCode::SOCKET_NOT_LISTENING;
  }

  return StartErrorCode::SUCCESS;
}

int HttpServer::UpdateConnections() {
  // TODO(Olster): Add paraeters here.
  return Socket::Select(m_maxFd + 1, readSet, writeSet, errorSet, &timeout);
}
} // namespace net
