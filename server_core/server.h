#ifndef NET_SERVER_H_
#define NET_SERVER_H_

#include <vector>

#include "base/build_required.h"
#include "base/path.h"
#include "socket/tcp_listener.h"
#include "server_plugin/plugin_loader.h"

class Session;

class Server {
 public:
  Server();
  ~Server();

  bool LoadPlugins(const Path::StringType& pluginsFolder);

  // Starts up controller (if registered) and plugins.
  void Run();

  void RegisterSession(Session* s);
  void UnloadAllPlugins();

 private:
  void InitPlugins();

  void FillReadyRead(fd_set* readSet);
  void FillReadyWrite(fd_set* writeSet);
  void FillError(fd_set* errorSet);

  void CloseOutdatedConnections();

  void ReadData(const fd_set& readSet);
  void SendData(const fd_set& writeSet);
  void ErrorSessions(const fd_set& errorSet);

  PluginLoader m_pluginLoader;

  std::vector<Session*> m_sessions;

  // NOTE(Olster): There's no point of setting max listen to more than
  // we can handle simultaneously in select().
  const int m_maxListen;

  unsigned int m_maxFd;

  fd_set m_masterSet;

  DISALLOW_COPY_AND_ASSIGN(Server);
  DISALLOW_MOVE(Server);
};

#endif  // NET_SERVER_H_
