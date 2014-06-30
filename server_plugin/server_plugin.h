#ifndef SERVER_PLUGIN_H_
#define SERVER_PLUGIN_H_

#include <queue>

#include "base/build_required.h"
#include "protocols/plugin_export.h"

// TODO(Olster): Include those here so user can just include this file.
class ProtocolHandler;
class IPEndPoint;

// TODO(Olster): Move anywhere else?
enum SockType {
  TCP = 0
};

class ServerPlugin {
 public:
  ServerPlugin() {}
  virtual ~ServerPlugin() {}

  virtual void ip_endpoint(IPEndPoint* ep) = 0;
  virtual SockType sock_type() = 0;

  virtual ProtocolHandler* NewProtocolHandler() = 0;
  virtual void FreeProtocolHandler(ProtocolHandler* handler) = 0;

  virtual const std::string& name() const = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(ServerPlugin);
  DISALLOW_MOVE(ServerPlugin);
};

#endif // SERVER_PLUGIN_H_
