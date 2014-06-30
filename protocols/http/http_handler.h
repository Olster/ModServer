#ifndef HTTP_PROTOCOL_HANDLER_H_
#define HTTP_PROTOCOL_HANDLER_H_

#include "protocols/protocol_handler.h"

class HttpHandler : public ProtocolHandler {
 public:
  HttpHandler() {}
  bool HasDataToSend() const override;

  void DidReceive(char* data, int size) override;
  void DidSend(int size) override;

  const char* data_to_send() override;
  size_t data_to_send_size() override;
 private:
  std::string m_request;
  std::string m_response;

  DISALLOW_COPY_AND_ASSIGN(HttpHandler);
  DISALLOW_MOVE(HttpHandler);
};

#endif // HTTP_PROTOCOL_HANDLER_H_
