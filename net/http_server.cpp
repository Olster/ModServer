#include "net/http_server.h"

#include <errno.h>
#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

namespace net {

HTTPServer::HTTPServer(int port, int pendingConnections, std::string filePath)
 : m_serverSock(port), m_resourcePath(filePath) {
#ifdef DEBUG
std::cout << "Info: Creating server" << std::endl;
#endif

  if (!m_serverSock.Open()) {
    m_strErrMsg = "Failed to open socket";
    return;
  }

  if (!m_serverSock.Bind()) {
    m_strErrMsg = "Failed to bind socket";
    return;
  }

  if (!m_serverSock.Listen(pendingConnections)) {
    m_strErrMsg = "Failed to listen on socket";
    return;
  }

  FD_ZERO(&m_masterSet);
  FD_ZERO(&m_readSet);
  FD_ZERO(&m_writeSet);

  FD_SET(m_serverSock.GetHandle(), &m_masterSet);

  // 5 min timeout for select()
  m_timeout.tv_sec = 5*60;
  m_timeout.tv_usec = 0;

  m_dMaxFileDescr = m_serverSock.GetHandle();

  m_bIsInErrorState = false;
}

HTTPServer::~HTTPServer() {
#ifdef DEBUG
std::cout << "Info: Destroying the server" << std::endl;
#endif
  for (auto& session : m_sessions) {
    delete session;
    session = nullptr;
  }
}

bool HTTPServer::UpdateSessions() {
#ifdef DEBUG
std::cout << "Info: Updating sessions" << std::endl;
#endif

  for (auto& removeItem : m_removeList) {
    FD_CLR(removeItem->GetReceiver()->GetHandle(), &m_masterSet);

    m_sessions.remove(removeItem);

    delete removeItem;
    removeItem = 0;
  }
  m_removeList.clear();

  m_readSet = m_masterSet;
  m_writeSet = m_masterSet;

  // Not going to write to ourselves
  FD_CLR(m_serverSock.GetHandle(), &m_writeSet);

  timeval timeLeft = m_timeout;

  int retCode = net::Socket::Select(m_dMaxFileDescr + 1, &m_readSet, &m_writeSet, nullptr, &timeLeft);

  if (retCode == 0) {
    m_strErrMsg = "Select() timed out";
    m_bIsInErrorState = true;

    return false;
  }

  if (retCode == -1) {
    m_strErrMsg = strerror(errno);
    m_bIsInErrorState = true;

    return false;
  }

  return true;
}

void HTTPServer::CloseTimedOutSessions() {
#ifdef DEBUG
std::cout << "Info: Closing timed out sessions" << std::endl;
#endif
  // TODO(Olster): Find a way to determine timed out connection
}

void HTTPServer::AcceptNewConnections() {
  if (FD_ISSET(m_serverSock.GetHandle(), &m_readSet)) {
    net::TCPSocket* sock = m_serverSock.Accept();

    if (sock->GetHandle() > m_dMaxFileDescr) {
      m_dMaxFileDescr = sock->GetHandle();
    }

    FD_SET(sock->GetHandle(), &m_masterSet);

#ifdef DEBUG
std::cout << "Info: Creating new session with " << sock->GetHandle() << std::endl;
#endif

    m_sessions.push_back(new net::TCPSession(sock, m_resourcePath));
  }
}

void HTTPServer::ProcessSessions() {
#ifdef DEBUG
std::cout << "Info: Processing sessions" << std::endl;
#endif
  for (auto& session : m_sessions) {
    if (session) {
      if (FD_ISSET(session->GetReceiver()->GetHandle(), &m_readSet)) {
        int retCode = session->Receive();
        if (retCode == 0) {
  #ifdef DEBUG
  std::cout << "Info: socket closed the connection" << std::endl;
  #endif
          m_removeList.push_back(session);
          continue;
        }

        if (retCode == -1) {
  #ifdef DEBUG
  std::cout << "Error: socket error whilst receiving" << std::endl;
  #endif
          m_removeList.push_back(session);
          continue;
        }
      }

      if (FD_ISSET(session->GetReceiver()->GetHandle(), &m_writeSet)) {
        session->Send();
      }
    }
  }
}

} // namespace net
