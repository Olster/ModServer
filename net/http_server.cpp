#include "net/http_server.h"

#include <assert.h>

#include "net/http_connection.h"

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
  // TODO(Olster): Add parameters here.
  m_readSet = m_masterSet;
  m_writeSet = m_masterSet;

  timeval timeout = m_timeout;
  return Socket::Select(m_maxFd + 1, &m_readSet, &m_writeSet, &m_errorSet, &timeout);
}

void HttpServer::AcceptNewConnections() {
  if (FD_ISSET(m_listenerSocket.handle())) {
    TcpSocket newSock = m_listenerSocket.Accept();
    assert(newSock);

    m_connections.push_back(new HttpConnection(newSock, m_resourcesFolderPath));
  }
}

void HttpServer::CloseUntrackedConnections() {
  auto it = m_connections.begin();
  while (it != m_connections.end()) {
    HttpConnection* conn = *it;
    if (FD_ISSET(conn->ClientHandle(), &m_errorSet) || conn->ReadyClose()) {
      delete conn;
      it = m_connections.erase(it);
      continue;
    }

    ++it;
  }
}
} // namespace net
