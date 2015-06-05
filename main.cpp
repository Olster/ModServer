#include "base/command_line.h"
#include "base/logger.h"
#include "server_core/server.h"

void PrintUsage() {
  printf("-serverIp=<IP> -- IP that server will be listening on.\n");
  printf("-pluginsFolder=<path> -- (optional) where server should look for plugins.\n");
  printf("-logToFile -- (optional) log all output to a file.\n");
}

int main(int argc, const char** argv) {
  if (argc <= 1) {
    PrintUsage();
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
