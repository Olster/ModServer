#include "net/server.h"
#include "base/logger.h"

int main(int argc, char** argv) {
  if (!Logger::InitLog()) {
    printf("Log file wasn't opened");
    return 1;
  }

  Server server;

  std::string pluginsPath = "plugins/";

#ifdef DEBUG
  pluginsPath = "Debug/" + pluginsPath;
#endif

  if (!server.LoadPlugins(pluginsPath)) {
    Logger::Log(Logger::ERR, "No plugins were loaded");
  }

  server.Run();
  
  Logger::UninitLog();
  return 0;
}
