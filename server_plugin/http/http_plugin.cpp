#include <cstring>

#include "plugin_api/plugin.h"

#include "server_plugin/http/http_handler.h"

void GetPluginName(char* buf, int size) {
#if defined(WIN32)
#define snprintf _snprintf
#endif
  snprintf(buf, size, "%s", "HTTP");
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
