#include "server_plugin/controller.h"

#include "base/logger.h"
#include "net/server.h"
#include "protocols/controller/controller_handler.h"

std::string Controller::m_name = "Controller";

Controller::Controller(Server* server)
 : m_server(server) {}

void Controller::ip_endpoint(IPEndPoint* ep) {
  ep->set_ip("127.0.0.1");
  ep->set_port(2563);
}
 
SockType Controller::sock_type() {
  return SockType::TCP;
}

ProtocolHandler* Controller::NewProtocolHandler() {
  return new ControllerHandler(m_server);
}
