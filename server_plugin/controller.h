#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "server_plugin/server_plugin.h"

class Server;

// Controller plugin is built-in inside of the server executable.
// With it we can update other plugins, remove plugins,
// close connections, etc.
class Controller : public ServerPlugin {
 public:
  Controller(Server* server);

  const char* ip() const override;
  unsigned short port() const override;

  SessionHandler* NewHandler() override;
};

#endif // CONTROLLER_H_
