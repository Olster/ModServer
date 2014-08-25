#include "base/command_line.h"
#include "base/logger.h"
#include "net/server.h"

int main(int argc, const char** argv) {
  if (argc <= 1) {
    // TODO(Olster): Print usage.
    printf("Please specify path to plugins folder");
    return 1;
  }

  if (!CommandLine::Init(argc, argv)) {
    perror("Command line was not initialized.");
    return 1;
  }

  CommandLine& cl = *CommandLine::ForCurrentProcess();
  if (!Logger::InitLog(cl.HasSwitch("logToFile"))) {
    perror("Log file wasn't opened");
    return 1;
  }

  Path::StringType pluginsPath = PATH_LITERAL(".");
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
    Log(ERR) << "No plugins were loaded, exiting.";
  }

  Logger::UninitLog();
  return 0;
}
