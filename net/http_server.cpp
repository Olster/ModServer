#include "net/http_server.h"

#include <assert.h>

#include "net/http_connection.h"
#include "net/ipendpoint.h"
#include "base/logger.h"

HttpServer::HttpServer(const IPEndPoint& endPoint, int maxListen)
 : m_listenerSocket(endPoint.ip(), endPoint.port()),
   m_maxListen(maxListen) {

  // 20 seconds default timeout.
  // TODO(Olster): Make this configurable.
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

HttpServer::UpdateCode HttpServer::UpdateConnections() {
  m_readSet = m_masterSet;
  m_errorSet = m_masterSet;

  FD_ZERO(&m_writeSet);
  for (const auto& conn : m_connections) {
    if (conn->DataAvailable()) {
      // Add socket for "ready write" check only if server has data to send.
      FD_SET(conn->clientSocket()->handle(), &m_writeSet);

      // If server has data to send on socket (hasn't sent all data before),
      // remove from ready read check.
      FD_CLR(conn->clientSocket()->handle(), &m_readSet);
    }
  }

  timeval timeout = m_timeout;
  int res = Socket::Select(m_maxFd + 1, &m_readSet, &m_writeSet, &m_errorSet, &timeout);

  UpdateCode outCode = OK;

  switch (res) {
    case 0:
      outCode = TIME_OUT;
    break;

    case -1:
      outCode = UPDATE_ERROR;
    break;

    default:
    break;
  }

  return outCode;
}

void HttpServer::AcceptNewConnections() {
  if (FD_ISSET(m_listenerSocket.handle(), &m_readSet)) {
    TcpSocket* newSock = m_listenerSocket.Accept();
    assert(newSock);

    m_connections.push_back(new HttpConnection(newSock, m_hostToLocalPath));

    // Update max FD variable.
    Socket::SOCK_TYPE sock = newSock->handle();
    Logger::Log("Socket %d connected", sock);
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

    // conn can only be null if there was no memory to allocate it.
    // Which would have thrown an exception.
    // TODO(Olster): Clear this up.
    assert(conn);

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
          Logger::Log("Socket %d closed the conenction. Read 0 bytes.",
                      clientSock->handle());
        } else {
          Logger::Log("Error on socket %d", clientSock->handle());
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
    // Don't try to send to sockets that are about to be closed.
    if (conn->ReadyClose()) {
      continue;
    }

    TcpSocket* clientSock = conn->clientSocket();
    if (FD_ISSET(clientSock->handle(), &m_writeSet)) {
      int sent = conn->SendResponse();

      // Need to close the connection: either error or close.
      if (sent < 1) {
        Logger::Log("Must close %d. Couldn't send data.", clientSock->handle());
        conn->SetReadyClose();
      }
    }
  }
}
