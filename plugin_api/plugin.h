#ifndef PLUGIN_API_PLUGIN_H_
#define PLUGIN_API_PLUGIN_H_

#include "plugin_api/plugin_export.h"

class ProtocolHandler;

#ifdef __cplusplus
extern "C" {
#endif

PLUGIN_EXPORT void GetPluginName(char* buf, int size);

typedef enum {
  TCP = 0
} SockType;

PLUGIN_EXPORT SockType SocketType();

PLUGIN_EXPORT unsigned short DefaultPort();

PLUGIN_EXPORT ProtocolHandler* NewHandler();

#ifdef __cplusplus
}
#endif

#endif  // PLUGIN_API_PLUGIN_H_
