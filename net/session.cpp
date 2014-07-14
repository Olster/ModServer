#include "net/session.h"

#include "net/socket/socket.h"
#include "protocols/protocol_handler.h"

Session::Session(Socket* sock, ProtocolHandler* protoHandler)
 : m_sock(sock),
   m_protoHandler(protoHandler) {}

Session::~Session() {
  delete m_sock;

  m_protoHandler->FreeHandler();
}
