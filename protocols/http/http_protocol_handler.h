#ifndef HTTP_PROTOCOL_HANDLER_H_
#define HTTP_PROTOCOL_HANDLER_H_

#include "protocols/protocol_handler.h"

class PROTOCOL_EXPORT HttpProtocolHandler : public ProtocolHandler {
 public:
  DISALLOW_COPY_AND_ASSIGN(HttpProtocolHandler);
  DISALLOW_MOVE(HttpProtocolHandler);

  HttpProtocolHandler() = default;

  bool HasDataToSend() const override;

  const std::string& protocol_name() const override;

  char* GetReadBuffer(int* returnedBufSize) override;

  void DidReceive(char* data, int size) override;

  char* data_to_send() override;
  size_t data_to_send_size() override;

  bool ShiftDataToSend(int bytes) override;

 private:
  std::string m_protoName = "HTTP";
};

#endif // HTTP_PROTOCOL_HANDLER_H_
