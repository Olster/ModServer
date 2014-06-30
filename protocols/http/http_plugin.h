#ifndef HTTP_PLUGIN_H_
#define HTTP_PLUGIN_H_

#include <string>

#include "server_plugin/server_plugin.h"

class HttpPlugin : public ServerPlugin {
 public:
  void ip_endpoint(IPEndPoint* ep) override;
  SockType sock_type() override { return TCP; }

  ProtocolHandler* NewProtocolHandler() override;
  void FreeProtocolHandler(ProtocolHandler* handler) override;

  const std::string& name() const override { return m_name; }
 private:
  static std::string m_name;
};

extern "C" {
  PLUGIN_EXPORT ServerPlugin* NewPluginInstance();
  PLUGIN_EXPORT void FreePluginInstance(ServerPlugin* plugin);
}

#endif // HTTP_PLUGIN_H_
