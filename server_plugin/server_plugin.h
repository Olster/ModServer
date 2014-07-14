#ifndef SERVER_PLUGIN_H_
#define SERVER_PLUGIN_H_

#include <string>

#include "base/build_required.h"
#include "server_plugin/plugin.h"

class ProtocolHandler;
class IPEndPoint;
class DynamicLib;

class ServerPlugin {
 public:
  ServerPlugin(DynamicLib* lib)
   : m_lib(lib) {}

  ~ServerPlugin();

  // Checks if the dynamic lib is a suitable plugin.
  bool IsValid();

  void ip_endpoint(IPEndPoint* ep);
  SockType sock_type();

  ProtocolHandler* NewProtocolHandler();

  const std::string& name();
 private:
  DynamicLib* m_lib;
  std::string m_name;

  DISALLOW_COPY_AND_ASSIGN(ServerPlugin);
  DISALLOW_MOVE(ServerPlugin);
};

#endif // SERVER_PLUGIN_H_
