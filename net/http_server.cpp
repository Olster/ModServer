#include "net/http_server.h"

#include <assert.h>
#include <iostream>

#include "net/http_connection.h"

namespace net {
HttpServer::HttpServer(const char* ip, unsigned short port,
                       const std::string& resFolder, int maxListen)
 : m_listenerSocket(ip, port),
   m_resourcesFolderPath(resFolder),
   m_maxListen(maxListen) {

  // 20 seconds default timeout.
  m_timeout.tv_sec = 20;
  m_timeout.tv_usec = 0;

  FD_ZERO(&m_masterSet);
  FD_ZERO(&m_readSet);
  FD_ZERO(&m_writeSet);
  FD_ZERO(&m_errorSet);
}

HttpServer::~HttpServer() {
  m_listenerSocket.ShutDown();
  m_listenerSocket.Close();

  while (!m_connections.empty()) {
    HttpConnection* conn = m_connections.front();
    m_connections.pop_front();

    delete conn;
  }
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

  // Add listening socket to the master list.
  FD_SET(m_listenerSocket.handle(), &m_masterSet);

  return StartErrorCode::SUCCESS;
}

int HttpServer::UpdateConnections() {
  m_readSet = m_masterSet;
  m_writeSet = m_masterSet;
  m_errorSet = m_masterSet;

  timeval timeout = m_timeout;
  return Socket::Select(m_maxFd + 1, &m_readSet, &m_writeSet, &m_errorSet, &timeout);
}

void HttpServer::AcceptNewConnections() {
  if (FD_ISSET(m_listenerSocket.handle(), &m_readSet)) {
    TcpSocket* newSock = m_listenerSocket.Accept();
    assert(newSock);

    m_connections.push_back(new HttpConnection(newSock, m_resourcesFolderPath));

    // Update max FD variable.
    SOCK_TYPE sock = newSock->handle();
    if (sock > m_maxFd) {
      m_maxFd = sock;
    }

    // Add sock to master set.
    FD_SET(sock, &m_masterSet);
  }
}

void HttpServer::CloseUntrackedConnections() {
  auto it = m_connections.begin();
  while (it != m_connections.end()) {
    HttpConnection* conn = *it;
    if (FD_ISSET(conn->clientSocket()->handle(), &m_errorSet) || conn->ReadyClose()) {
      FD_CLR(conn->clientSocket()->handle(), &m_masterSet);
      delete conn;

      it = m_connections.erase(it);
      continue;
    }

    ++it;
  }
}

void HttpServer::ReadRequests() {
  for (auto& conn : m_connections) {
    TcpSocket* clientSock = conn->clientSocket();
    if (FD_ISSET(clientSock->handle(), &m_readSet)) {
      int read = conn->ReadRequest();
      
      // Error reading or closed connection, mark connection for close.
      if (read < 1) {
        if (read == 0) {
          std::cout << "Socket " << clientSock->handle() << " closed the conenction" << std::endl;
        } else {
          std::cout << "Error on socket " << clientSock->handle() << std::endl;
        }

        conn->SetReadyClose();
      }
    }
  }
}

void HttpServer::ProcessRequests() {
  for (auto& conn : m_connections) {
    conn->ProcessRequest();
  }
}

void HttpServer::SendResponses() {
  for (auto& conn : m_connections) {
    TcpSocket* clientSock = conn->clientSocket();
    if (FD_ISSET(clientSock->handle(), &m_writeSet) && conn->DataAvailable()) {
      int sent = conn->SendResponse();
      
      // Need to close the connection: either error or close.
      if (sent < 1) {
        std::cout << "closing socket " << clientSock->handle() << std::endl;
        conn->SetReadyClose();
      }
    }
  }
}

} // namespace net
