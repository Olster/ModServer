#include "net/server.h"

#include <assert.h>

#include "base/logger.h"
#include "server_plugin/controller.h"

Server::Server() {
  FD_ZERO(&m_masterSet);
}

Server::~Server() {
  m_pluginLoader.UnloadAll();
}

bool Server::LoadPlugins() {
  if (!m_pluginLoader.HasLoadedPlugins()) {
    Logger::Log(Logger::INFO, "Plugins were already loaded, don't load again");
    return false;
  }

  m_pluginLoader.LoadAll();

  // Controller plugin controls the server.
  m_pluginLoader.AddPlugin(new Controller(this));

  return true;
}

void Server::Run() {
  StartPlugins();

  while (true) {
    fd_set readSet;
    FD_ZERO(&readSet);
    FillReadyRead(&readSet);

    fd_set writeSet;
    FD_ZERO(&writeSet);
    FillReadyWrite(&writeSet);

    fd_set errorSet;
    FD_ZERO(&errorSet);
    FillError(&writeSet);    

    // 5 minutes default timeout.
    // TODO(Olster): Make this configurable.
    timeval timeout = {0};
    timeout.tv_sec = 5*60;

    int res = Socket::Select(m_maxFd + 1, &readSet, &writeSet, &errorSet, &timeout);

    CloseOutdatedConnections();

    ReadData();
    SendData();
  }
}

void Server::StartPlugins() {

}

void Server::FillReadyRead(fd_set* readSet) {

}

void Server::FillReadyWrite(fd_set* writeSet) {

}

void Server::FillError(fd_set* errorSet) {

}

void Server::CloseOutdatedConnections() {

}

void Server::ReadData() {

}

void Server::SendData() {

}
