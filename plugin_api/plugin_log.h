#ifndef PLUGIN_API_LOGGING_H_
#define PLUGIN_API_LOGGING_H_

#include <fstream>

enum LogSeverity {
  VERBOSE = 0,
  INFO,
  WARN,
  ERR
};

#if defined(PLUGIN_IMPLEMENTATION)
std::ostream& PluginLog(LogSeverity sev);
#endif

#endif  // PLUGIN_API_LOGGING_H_
