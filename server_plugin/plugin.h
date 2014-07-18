#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "server_plugin/plugin_export.h"

class ProtocolHandler;

#ifdef __cplusplus
extern "C" {
#endif

PLUGIN_EXPORT void GetPluginName(char* buf, int size);

typedef enum {
  TCP = 0
} SockType;

PLUGIN_EXPORT SockType SocketType();

PLUGIN_EXPORT void DefaultIPv4(char* ip, int maxLen);
PLUGIN_EXPORT unsigned short DefaultPort();

PLUGIN_EXPORT ProtocolHandler* NewHandler();

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H_
