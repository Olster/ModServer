#include <cstring>

#include "server_plugin/plugin.h"
#include "net/ip_endpoint.h"

#include "server_plugin/http/http_handler.h"

void GetPluginName(char* buf, int /*size*/) {
  sprintf(buf, "%s", "HTTP");
}

SockType SocketType() {
  return SockType::TCP;
}

void DefaultIPv4(char* ip, int /*maxLen*/) {
  strcpy(ip, "127.0.0.1");
}

unsigned short DefaultPort() {
  return 80;
}

void FreeHandler(ProtocolHandler* handler) {
  delete handler;
}

ProtocolHandler* NewHandler() {
  return new HttpHandler(FreeHandler);
}
