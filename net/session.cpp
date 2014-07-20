#include "net/session.h"

#include "net/socket/socket.h"
#include "server_plugin/protocol_handler.h"

Session::Session(std::shared_ptr<Socket> sock,
                 ProtocolHandler* protoHandler)
 : m_sock(sock),
   m_protoHandler(protoHandler) {}

Session::~Session() {
  m_protoHandler->FreeHandler();
}
