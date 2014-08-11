#include "net/server.h"
#include "base/logger.h"

int main(int argc, Path::StringType::value_type** argv) {
  if (argc <= 1) {
    printf("Please specify path to plugins folder");
    return 1;
  }

  if (!Logger::InitLog()) {
    printf("Log file wasn't opened");
    return 1;
  }

  Path::StringType pluginsPath = argv[1];
  if (pluginsPath[pluginsPath.length() - 1] != '/') {
    pluginsPath += '/';
  }

  pluginsPath += PATH_LITERAL("plugins/");

  Server server;
  if (server.LoadPlugins(pluginsPath)) {
    server.Run();
  } else {
    Logger::Log(Logger::ERR, "No plugins were loaded, exiting.");
  }

  Logger::UninitLog();
  return 0;
}
