#include "protocols/http/http_protocol_handler.h"

// TODO(Olster): Implement this class.
bool HttpProtocolHandler::HasDataToSend() const {
  return false;
}

const std::string& HttpProtocolHandler::protocol_name() const {
  return m_protoName;
}

char* HttpProtocolHandler::GetReadBuffer(int* returnedBufSize) {
  UNUSED(returnedBufSize);
  return nullptr;
}

void HttpProtocolHandler::DidReceive(char* data, int size) {
  UNUSED(data);
  UNUSED(size);
}

char* HttpProtocolHandler::data_to_send() {
  return nullptr;
}

size_t HttpProtocolHandler::data_to_send_size() {
  return 0;
}

bool HttpProtocolHandler::ShiftDataToSend(int bytes) {
  UNUSED(bytes);
  return false;
}