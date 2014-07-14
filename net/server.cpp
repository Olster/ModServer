#include "net/server.h"

#include <assert.h>
#include <functional>

#include "base/logger.h"
//#include "server_plugin/controller.h"
#include "server_plugin/server_plugin.h"

#include "net/tcp_session.h"

Server::Server()
 : m_maxListen(FD_SETSIZE - 2),
   m_maxFd(0) {
  FD_ZERO(&m_masterSet);
}

Server::~Server() {
  m_pluginLoader.UnloadAll();
}

bool Server::LoadPlugins(const std::string& pluginsFolder) {
  if (m_pluginLoader.HasLoadedPlugins()) {
    Logger::Log(Logger::INFO, "Plugins were already loaded, don't load again");
    return false;
  }

  m_pluginLoader.LoadAll(pluginsFolder);

  // Controller plugin controls the server.

  //PluginData* pluginData = new PluginData;
  //pluginData->plugin = new Controller(this);
  //pluginData->freeFn = FreeController;
  //
  //m_pluginLoader.AddPlugin(pluginData);

  return true;
}

void Server::Run() {
  InitPlugins();

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

    int res = Socket::Select(m_maxFd, &readSet, &writeSet, &errorSet, &timeout);
    CloseOutdatedConnections();

    switch (res) {
      case 0:
        Logger::Log(Logger::INFO, "Select timed out");
      break;

      case -1:
        // TODO(Olster): Report actual error.
        Logger::Log(Logger::ERR, "Select error %d", errno);
        return;
      break;

      default:
      break;
    }

    ReadData(readSet);
    SendData(writeSet);
  }
}

void Server::RegisterSession(Session* s) {
  assert(s);
  m_sessions.push_back(s);
}

void Server::UnloadAllPlugins() {
  m_pluginLoader.UnloadAll();
}

void Server::InitPlugins() {
  std::list<ServerPlugin*> plugins;
  m_pluginLoader.GetPlugins(&plugins);

  for (ServerPlugin* plugin : plugins) {
    Logger::Log(Logger::INFO, "Initializing plugin: %s", plugin->name().c_str());

    IPEndPoint ep;
    plugin->ip_endpoint(&ep);

    if (!ep.IsValid()) {
      Logger::Log(Logger::WARN, "Plugin %s returned invalid IPEndPoint: %s:%d",
                  plugin->name().c_str(), ep.ip(), ep.port());

      continue;
    }

    Logger::Log(Logger::INFO, "Plugin %s will serve connections on %s:%d", plugin->name().c_str(), ep.ip(), ep.port());

    SockType type = plugin->sock_type();
    switch (type)
    {
      case TCP: {
        TcpListener* newSock = new TcpListener(ep);

        if (!newSock->Open()) {
          Logger::Log(Logger::ERR, "Socket wasn't opened %s", plugin->name().c_str());
          delete newSock;
          continue;
        }

        if (!newSock->Bind()) {
          Logger::Log(Logger::ERR, "Socket wasn't bound %s", plugin->name().c_str());
          delete newSock;
          continue;
        }

        if (!newSock->Listen(m_maxListen)) {
          Logger::Log(Logger::ERR, "Socket isn't listening %s", plugin->name().c_str());
          delete newSock;
          continue;
        }

        AcceptorSession* acceptorSession = new AcceptorSession(newSock, NULL, this, plugin);
        
        RegisterSession(acceptorSession);
      }
      break;

      // TODO(Olster): Add UDP support.
      default:
        Logger::Log(Logger::WARN, "Unsupported socket type");
      break;
    }
  }
}

void Server::FillReadyRead(fd_set* readSet) {
  for (auto& session : m_sessions) {
    if (session->CanRead()) {
      FD_SET(session->socket()->handle(), readSet);
    }
  }
}

void Server::FillReadyWrite(fd_set* writeSet) {
  for (auto& session : m_sessions) {
    if (session->HasDataToSend()) {
      FD_SET(session->socket()->handle(), writeSet);
    }
  }
}

void Server::FillError(fd_set* /*errorSet*/) {}

void Server::CloseOutdatedConnections() {}

namespace {
template <class T>
void RemoveFromVector(std::vector<T>& v, T elem) {
  v.erase(std::find(v.begin(), v.end(), elem));
}
}

void Server::ReadData(const fd_set& readSet) {
  // Copy these so no iterator invalidations occur.
  std::list<Session*> readSessions;

  for (Session* session : m_sessions) {
    if (FD_ISSET(session->socket()->handle(), &readSet)) {
      readSessions.push_back(session);
    }
  }

  for (Session* session : readSessions) {
    int read = session->OnRead();
    
    if (read < 1) {
      switch (read) {
        case 0:
          Logger::Log(Logger::INFO, "Socket closed %d", session->socket()->handle());
        break;

        case -1:
          Logger::Log(Logger::INFO, "Socket error %d", session->socket()->handle());
        break;

        default:
          Logger::Log(Logger::INFO, "Socket error < -1 %d", session->socket()->handle());
        break;
      }

      RemoveFromVector(m_sessions, session);
      delete session;
    }
  }
}

void Server::SendData(const fd_set& writeSet) {
  std::list<Session*> sendSessions;

  for (Session* session : m_sessions) {
    if (FD_ISSET(session->socket()->handle(), &writeSet)) {
      sendSessions.push_back(session);
    }
  }

  for (Session* session : sendSessions) {
    int wrote = session->OnWrite();
    if (wrote < 1) {
      switch (wrote) {
        case 0:
          Logger::Log(Logger::INFO, "Socket sent 0b on write %d", session->socket()->handle());
        break;

        case -1:
          Logger::Log(Logger::INFO, "Socket error on write %d", session->socket()->handle());
        break;

        default:
          Logger::Log(Logger::INFO, "Socket error < -1 on write %d", session->socket()->handle());
        break;
      }

      RemoveFromVector(m_sessions, session);
      delete session;
    }
  }
}

void Server::ErrorSessions(const fd_set& errorSet) {
  std::list<Session*> errSessions;

  for (Session* session : m_sessions) {
    if (FD_ISSET(session->socket()->handle(), &errorSet)) {
      errSessions.push_back(session);
    }
  }

  for (Session* session : errSessions) {
    session->OnError();
    RemoveFromVector(m_sessions, session);
  }
}
