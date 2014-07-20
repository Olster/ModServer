#include "net/server.h"
#include "base/logger.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    printf("Please specify path to plugins folder");
    return 1;
  }

  if (!Logger::InitLog()) {
    printf("Log file wasn't opened");
    return 1;
  }

  Server server;

  std::string pluginsPath = argv[1];
  if (pluginsPath[pluginsPath.length() - 1] != '/') {
    pluginsPath += '/';
  }

#ifdef DEBUG
  pluginsPath += "Debug/";
#endif
  
  pluginsPath += "plugins/";

  if (!server.LoadPlugins(pluginsPath)) {
    Logger::Log(Logger::ERR, "No plugins were loaded, exiting.");
    Logger::UninitLog();
    return 1;
  }

  server.Run();
  
  Logger::UninitLog();
  return 0;
}
