#include <cstring>

#include "plugin_api/plugin.h"
#include "net/ip_endpoint.h"

#include "server_plugin/http/http_handler.h"

int GetPluginName(char* buf, int maxSizeBytes) {
#if defined(WIN32)
#define snprintf _snprintf
#endif
  return snprintf(buf, maxSizeBytes, "%s", "HTTP");
}

SockType SocketType() {
  return SockType::TCP;
}

unsigned short DefaultPort() {
  return 8080;
}

void FreeHandler(ProtocolHandler* handler) {
  delete handler;
}

ProtocolHandler* NewHandler() {
  return new HttpHandler(FreeHandler);
}
