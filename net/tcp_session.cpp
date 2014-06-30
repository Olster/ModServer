#include "net/tcp_session.h"

#include "base/logger.h"
#include "net/socket/tcp_listener.h"
#include "protocols/protocol_handler.h"
#include "net/server.h"
#include "server_plugin/server_plugin.h"

bool ConnectionSession::CanRead() {
  return !m_protoHandler->HasDataToSend();
}

bool ConnectionSession::HasDataToSend() {
  return m_protoHandler->HasDataToSend();
}

int ConnectionSession::OnWrite() {
  int sent = reinterpret_cast<TcpSocket*>(m_sock)->Send(m_protoHandler->data_to_send(),
                                                        m_protoHandler->data_to_send_size());

  if (sent > 0) {
    m_protoHandler->DidSend(sent);
  }

  return sent;
}

int ConnectionSession::OnRead() {
  // TODO(Olster): Avoid copying.

  const int requestBufSize = 512;
  char buf[requestBufSize + 1];

  // TODO(Olster): Provide abstraction without casting to specific type.
  int bytesRead = reinterpret_cast<TcpSocket*>(m_sock)->Receive(buf, requestBufSize);
  if (bytesRead < 1) {
    return bytesRead;
  }

  buf[bytesRead] = '\0';

  m_protoHandler->DidReceive(buf, bytesRead);
  return bytesRead;
}

int AcceptorSession::OnRead() {
  Socket::SOCK_TYPE sock = reinterpret_cast<TcpListener*>(m_sock)->Accept();
  ConnectionSession* acceptedSession = new ConnectionSession(new TcpSocket(sock), m_plugin->NewProtocolHandler());

  Logger::Log(Logger::INFO, "Socket connected: %d. Handled by plugin: \"%s\"", sock, m_plugin->name().c_str());

  m_server->RegisterSession(acceptedSession);

  return 1;
}
