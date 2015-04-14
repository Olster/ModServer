#include "base/command_line.h"
#include "base/logger.h"
#include "server_core/server.h"

int main(int argc, const char** argv) {
  if (argc <= 1) {
    // TODO(Olster): Print usage.
    printf("Please specify path to plugins folder\n");
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

  if (!cl.HasSwitch("serverIp")) {
    Log(ERR) << "Please specify server IP\n";
    return 1;
  }

  int sockInit = Socket::InitSockets();
  if (sockInit == 0) {
    Server server;
    if (server.LoadPlugins(pluginsPath)) {
      server.Run();
    } else {
      Log(ERR) << "No plugins were loaded, exiting.";
    }
  } else {
    Log(ERR) << "Sockets were not initialized.";
  }

  Socket::UninitSockets();
  Logger::UninitLog();
  return 0;
}
