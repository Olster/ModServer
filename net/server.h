#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include <vector>

#include "base/build_required.h"
#include "net/socket/tcp_listener.h"

class Server {
 public:
  DISALLOW_COPY_AND_ASSIGN(Server);
  DISALLOW_MOVE(Server);

  Server();
  ~Server();

  bool LoadPlugins();

  // Starts up controller (if registered) and plugins.
  void Run();
 private:
  void StartPlugins();

  void FillReadyRead(fd_set* readSet);
  void FillReadyWrite(fd_set* writeSet);
  void FillError(fd_set* errorSet);

  void CloseOutdatedConnections();

  void ReadData();
  void SendData();
  
  PluginLoader m_pluginLoader;

  std::vector<

  // NOTE(Olster): There's no point of setting max listen to more than
  // we can handle simultaneously in select().
  const int m_maxListen = FD_SETSIZE - 2;

  unsigned int m_maxFd = 0;

  fd_set m_masterSet;
};

#endif // NET_HTTP_SERVER_H_
