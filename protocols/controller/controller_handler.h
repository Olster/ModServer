#ifndef CONTROLLER_HANDLER_H_
#define CONTROLLER_HANDLER_H_

#include "protocols/protocol_handler.h"

#include <string>

class Server;

class ControllerHandler : public ProtocolHandler {
 public:
  ControllerHandler(Server* s)
   : m_server(s) {}

  bool HasDataToSend() const override { return false; }

  // Appends to the data received from server.
  void DidReceive(char* data, int size) override;
  void DidSend(int /*size*/) override {}

  const char* data_to_send() override { return NULL; }
  size_t data_to_send_size() override { return 0; }
 private:
  std::string m_data;

  Server* m_server;
};

#endif // CONTROLLER_HANDLER_H_
