#include "net/session.h"

#include "net/socket/socket.h"
#include "protocols/protocol_handler.h"

Session::Session(Socket* sock, ProtocolHandler* protoHandler)
 : m_sock(sock),
   m_protoHandler(protoHandler) {}

Session::~Session() {
  delete m_sock;

  // TODO(Olster): Make plugin free the handler.
  delete m_protoHandler;

  //if (m_protoHandler) {
  //  ProtocolHandler::Free(m_protoHandler);
  //}
}
