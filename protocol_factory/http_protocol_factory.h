#ifndef HTTP_PROTOCOL_FACTORY_H_
#define HTTP_PROTOCOL_FACTORY_H_

#include "protocol_factory/protocol_factory.h"

class HttpProtocolFactory : public ProtocolFactory {
 public:
  DISALLOW_COPY_AND_ASSIGN(HttpProtocolFactory);
  DISALLOW_MOVE(HttpProtocolFactory);

  HttpProtocolFactory() = default;

  ProtocolHandler* GetHandler() override;
};

#endif // HTTP_PROTOCOL_FACTORY_H_
