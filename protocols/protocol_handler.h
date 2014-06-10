#ifndef PROTOCOL_HANDLER_H_
#define PROTOCOL_HANDLER_H_

#include <string>

#include "base/build_required.h"
#include "protocols/protocol_export.h"

class PROTOCOL_EXPORT ProtocolHandler {
 public:
  DISALLOW_COPY_AND_ASSIGN(ProtocolHandler);
  DISALLOW_MOVE(ProtocolHandler);

  ProtocolHandler() = default;
  virtual ~ProtocolHandler() {}

  virtual bool HasDataToSend() const = 0;

  virtual const std::string& protocol_name() const = 0;

  // Returns buffer which we read data into, sets |returnedBufSize| to the
  // buffer size.
  virtual char* GetReadBuffer(int* returnedBufSize) = 0;

  // Appends to the data received from server. |data| must be allocated on
  // heap, it will be deleted.
  virtual void DidReceive(char* data, int size) = 0;

  virtual char* data_to_send() = 0;
  virtual size_t data_to_send_size() = 0;

  // Shifts left data to send by |bytes|. Used when not all data were sent.
  virtual bool ShiftDataToSend(int bytes) = 0;
};

#endif // PROTOCOL_HANDLER_H_
