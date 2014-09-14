#include "plugin_api/plugin_log.h"

#include <iostream>

#include "plugin_api/plugin.h"

#if defined(PLUGIN_IMPLEMENTATION)

namespace {
LogFnType LogFn = NULL;
}  // namespace

std::ostream& PluginLog(LogSeverity sev) {
  if (LogFn) {
    return LogFn(sev);
  }

  return std::cerr;
}

void RegisterLogFn(LogFnType logFn) {
  LogFn = logFn;
}

#endif
