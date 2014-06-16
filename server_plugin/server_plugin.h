#ifndef SERVER_PLUGIN_H_
#define SERVER_PLUGIN_H_

#include <string>

#include "base/build_required.h"

class SessionHandler;
class Socket;

class ServerPlugin {
 public:
  DISALLOW_COPY_AND_ASSIGN(ServerPlugin);
  DISALLOW_MOVE(ServerPlugin);

  ServerPlugin() {}
  virtual ~ServerPlugin() {}

  virtual Socket* SocketToRegister() = 0;
  virtual SessionHandler* NewHandler() = 0;
};

#endif // SERVER_PLUGIN_H_
