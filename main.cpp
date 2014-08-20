#include "base/command_line.h"
#include "base/logger.h"
#include "net/server.h"

int main(int argc, const char** argv) {
  if (argc <= 1) {
    printf("Please specify path to plugins folder");
    return 1;
  }

  if (!CommandLine::Init(argc, argv)) {
    perror("Command line was not initialized.");
    return 1;
  }

  if (!Logger::InitLog()) {
    perror("Log file wasn't opened");
    return 1;
  }

  Path::StringType pluginsPath = PATH_LITERAL(".");

  CommandLine& cl = *CommandLine::ForCurrentProcess();
  if (cl.HasSwitch("pluginsFolder")) {
    pluginsPath = cl.SwitchValue("pluginsFolder");
  }

  if (pluginsPath[pluginsPath.length() - 1] != '/') {
    pluginsPath += '/';
  }

  Server server;
  if (server.LoadPlugins(pluginsPath)) {
    server.Run();
  } else {
    Logger::Log(Logger::ERR, "No plugins were loaded, exiting.");
  }

  Logger::UninitLog();
  return 0;
}
