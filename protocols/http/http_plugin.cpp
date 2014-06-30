#include "protocols/http/http_plugin.h"
#include "net/ip_endpoint.h"

#include "protocols/http/http_handler.h"

std::string HttpPlugin::m_name = "HTTP plugin";

void HttpPlugin::ip_endpoint(IPEndPoint* ep) {
  ep->set_ip("127.0.0.1");
  ep->set_port(80);
}

ProtocolHandler* HttpPlugin::NewProtocolHandler() {
  return new HttpHandler();
}

void HttpPlugin::FreeProtocolHandler(ProtocolHandler* handler) {
  delete handler;
}

ServerPlugin* NewPluginInstance() {
  return new HttpPlugin();
}

void FreePluginInstance(ServerPlugin* plugin) {
  delete plugin;
}
