#ifndef SERVER_PLUGIN_PROTOCOL_HANDLER_H_
#define SERVER_PLUGIN_PROTOCOL_HANDLER_H_

#include <string>

class ProtocolHandler {
 public:
  typedef void (*FreeFunc)(ProtocolHandler* handler);

  // Free func defines how |this| gets deleted.
  explicit ProtocolHandler(FreeFunc freeFunc)
      : m_freeFunc(freeFunc) {}

  virtual bool HasDataToSend() const = 0;

  // Appends to the data received from server.
  virtual void DidReceive(char* data, int size) = 0;

  // Notifies handler how much data was sent.
  virtual void DidSend(int size) = 0;

  virtual const char* data_to_send() = 0;
  virtual size_t data_to_send_size() = 0;

  // Since protocol handler are exported from DLLs,
  // they can't be just deleted. DLL passes deleter function when
  // creating the object so it can be cleaned up however DLL wants.
  void FreeHandler() {
    m_freeFunc(this);
  }

 protected:
  virtual ~ProtocolHandler() {}
  friend void FreeHandler(ProtocolHandler* handler);

 private:
  FreeFunc m_freeFunc;

  ProtocolHandler(const ProtocolHandler&);
  ProtocolHandler& operator=(const ProtocolHandler&);
  
  ProtocolHandler(ProtocolHandler&&);
  ProtocolHandler& operator=(ProtocolHandler&&);
};

#endif  // SERVER_PLUGIN_PROTOCOL_HANDLER_H_
