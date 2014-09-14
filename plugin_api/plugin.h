#ifndef PLUGIN_API_PLUGIN_H_
#define PLUGIN_API_PLUGIN_H_

#include "plugin_api/plugin_log.h"
#include "plugin_api/plugin_export.h"

class ProtocolHandler;

enum SockType {
  TCP = 0
};

#ifdef __cplusplus
extern "C" {
#endif

// Plugin fills |buf| with UTF-8 encoded name of the plugin,
// |buf| has a capacity of |maxSizeBytes| bytes.
PLUGIN_EXPORT int GetPluginName(char* buf, int maxSizeBytes);

// Plugin returns Socket type it's going to use.
// See the enum above for suported types.
PLUGIN_EXPORT SockType SocketType();

// TODO(Olster): Notify oplugin that the port is occupied.
// Plugin returns the default port it's going to serve on.
PLUGIN_EXPORT unsigned short DefaultPort();

// Plugin returns an instance of the protocol handler for
// the new connection.
PLUGIN_EXPORT ProtocolHandler* NewHandler();

// Host registers its Log function for use in plugin,
// so all logs are written to one log.
typedef std::ostream& (*LogFnType)(LogSeverity);
PLUGIN_EXPORT void RegisterLogFn(LogFnType logFn);

#ifdef __cplusplus
}
#endif

#endif  // PLUGIN_API_PLUGIN_H_
