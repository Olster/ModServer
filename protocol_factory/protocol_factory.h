#ifndef PROTOCOL_FACTORY_H_
#define PROTOCOL_FACTORY_H_

#include "base/build_required.h"

class ProtocolHandler;

class ProtocolFactory {
 public:
  DISALLOW_COPY_AND_ASSIGN(ProtocolFactory);
  DISALLOW_MOVE(ProtocolFactory);

  ProtocolFactory() = default;
  virtual ~ProtocolFactory() {}

  virtual ProtocolHandler* GetHandler() = 0;
};

#endif // PROTOCOL_FACTORY_H_
