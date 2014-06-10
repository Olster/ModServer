#include "net/server.h"

#include <assert.h>

#include "net/ip_endpoint.h"
#include "base/logger.h"
#include "protocol_factory/protocol_factory.h"
#include "protocols/protocol_handler.h"

Server::Server(const IPEndPoint& endPoint,
               ProtocolFactory* protFactory)
 : m_listener(endPoint.ip(), endPoint.port()),
   m_protoFactory(protFactory) {

  // 5 minutes default timeout.
  // TODO(Olster): Make this configurable.
  m_timeout.tv_sec = 5*60;
  m_timeout.tv_usec = 0;

  FD_ZERO(&m_masterSet);
  FD_ZERO(&m_readSet);
  FD_ZERO(&m_writeSet);
  FD_ZERO(&m_errorSet);
}

Server::~Server() {
  m_listener.ShutDown();
  m_listener.Close();

  // Clean up sockets and protocol handlers.
  for (auto& session : m_sessions) {
    delete session.first;
    delete session.second;
  }
}

Server::StartErrorCode Server::Start() {
  // TODO(Olster): Format error string if any errors occur.
  if (!m_listener.Open()) {
    return StartErrorCode::SOCKET_NOT_OPENED;
  }

  if (!m_listener.Bind()) {
    return StartErrorCode::SOCKET_NOT_BOUND;
  }

  if (!m_listener.Listen(m_maxListen)) {
    return StartErrorCode::SOCKET_NOT_LISTENING;
  }

  // Add listening socket to the master list.
  FD_SET(m_listener.handle(), &m_masterSet);

  return StartErrorCode::SUCCESS;
}

Server::UpdateCode Server::UpdateConnections() {
  m_readSet = m_masterSet;
  m_errorSet = m_masterSet;

  FD_ZERO(&m_writeSet);
  for (const auto& session : m_sessions) {
    if (session.second->HasDataToSend()) {
      // Add socket for "ready write" check only if server has data to send.
      FD_SET(session.first->handle(), &m_writeSet);

      // TODO(Olster): Does this hold for every protocol?
      // If server has data to send on socket (hasn't sent all data before),
      // remove from ready read check.
      FD_CLR(session.first->handle(), &m_readSet);
    }
  }

  timeval timeout = m_timeout;
  int res = Socket::Select(m_maxFd + 1, &m_readSet, &m_writeSet,
                           &m_errorSet, &timeout);

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

void Server::AcceptNewConnections() {
  if (FD_ISSET(m_listener.handle(), &m_readSet)) {
    TcpSocket* client = new TcpSocket(m_listener.Accept());

    ProtocolHandler* handler = m_protoFactory->GetHandler();
    m_sessions.insert(std::make_pair(client, handler));

    Socket::SOCK_TYPE sock = client->handle();
    Logger::Log("Socket %d connected, protocol: %s", sock,
                handler->protocol_name().c_str());

    // Update max FD variable.
    if (sock > m_maxFd) {
      m_maxFd = sock;
    }

    // Add sock to master set.
    FD_SET(sock, &m_masterSet);
  }
}

void Server::ReadRequests() {
  auto it = m_sessions.begin();

  while (it != m_sessions.end()) {
    TcpSocket* clientSock = it->first;
    if (FD_ISSET(clientSock->handle(), &m_readSet)) {
      ProtocolHandler* handler = it->second;

      int sizeToRead = 0;
      char* readBuffer = handler->GetReadBuffer(&sizeToRead);

      int bytesRead = clientSock->Receive(readBuffer, sizeToRead);
      if (bytesRead < 1) {
        if (bytesRead == 0) {
          Logger::Log("Socket %d closed the conenction. Read 0 bytes.",
                      clientSock->handle());
        } else {
          Logger::Log("Error on socket %d", clientSock->handle());
        }

        delete it->second;
        delete it->first;

        it = m_sessions.erase(it);
        continue;
      }

      handler->DidReceive(readBuffer, bytesRead);
      ++it;
    }
  }
}

void Server::SendResponses() {
  auto it = m_sessions.begin();

  while (it != m_sessions.end()) {
    TcpSocket* clientSock = it->first;
    if (FD_ISSET(clientSock->handle(), &m_writeSet)) {
      ProtocolHandler* handler = it->second;

      if (handler->HasDataToSend()) {
        int bytesSent = clientSock->Send(handler->data_to_send(),
                                         handler->data_to_send_size());

        if (bytesSent < 1) {
          Logger::Log("Bytes sent to %d was < 1: %d", clientSock->handle(),
                      bytesSent);

          delete it->second;
          delete it->first;

          it = m_sessions.erase(it);
        } else {
          handler->ShiftDataToSend(bytesSent);
          ++it;
        }        
      }
    }
  }
}
