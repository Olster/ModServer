#include "protocol_factory/http_protocol_factory.h"

#include "protocols/http/http_protocol_handler.h"

ProtocolHandler* HttpProtocolFactory::GetHandler() {
  return new HttpProtocolHandler;
}