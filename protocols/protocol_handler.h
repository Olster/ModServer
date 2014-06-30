#ifndef PROTOCOL_HANDLER_H_
#define PROTOCOL_HANDLER_H_

#include <string>

#include "base/build_required.h"

class ProtocolHandler {
 public:
  ProtocolHandler() {}
  virtual ~ProtocolHandler() {}

  virtual bool HasDataToSend() const = 0;

  // Appends to the data received from server.
  virtual void DidReceive(char* data, int size) = 0;

  // Notifies handler how much data was sent.
  virtual void DidSend(int size) = 0;

  virtual const char* data_to_send() = 0;
  virtual size_t data_to_send_size() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(ProtocolHandler);
  DISALLOW_MOVE(ProtocolHandler);
};

#endif // PROTOCOL_HANDLER_H_
