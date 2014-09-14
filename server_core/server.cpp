#include "server_core/server.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "base/logger.h"
#include "server_core/tcp_session.h"
//#include "server_plugin/controller.h"
#include "server_plugin/server_plugin.h"


Server::Server()
     : m_maxListen(FD_SETSIZE - 2),
     m_maxFd(0) {
  FD_ZERO(&m_masterSet);
}

Server::~Server() {
  m_pluginLoader.UnloadAll();
}

bool Server::LoadPlugins(const Path::StringType& pluginsFolder) {
  if (m_pluginLoader.HasLoadedPlugins()) {
    Log(INFO) << "Plugins were already loaded, don't load again";
    return false;
  }

  Log(INFO) << "Loading plugins from " << pluginsFolder;
  m_pluginLoader.LoadAll(pluginsFolder);

  return m_pluginLoader.HasLoadedPlugins();
}

void Server::Run() {
  InitPlugins();

  if (m_sessions.empty()) {
    Log(WARN) << "No sessions were registered.";
    return;
  }

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
        Log(INFO) << "Select timed out";
      break;

      case -1:
        Log(ERR) << "Select error " << errno;
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

  // Update the maxFd for select() to work.
  Socket::SOCK_TYPE sockId = s->socket()->handle();
  if (sockId >= m_maxFd) {
      m_maxFd = sockId + 1;
  }

  m_sessions.push_back(s);
}

void Server::UnloadAllPlugins() {
  m_pluginLoader.UnloadAll();
}

void Server::InitPlugins() {
  const std::list<ServerPlugin*>& plugins = m_pluginLoader.GetPlugins();

  for (ServerPlugin* plugin : plugins) {
    Log(INFO) << "Initializing plugin: " << plugin->name();

    plugin->Initialize();

    // TODO(Olster): Read IP from settings or command line.
    IPEndPoint ep("127.0.0.1", 0);
    ep.set_port(plugin->port());

    if (!ep.IsValid()) {
      Log(WARN) << "Plugin " << plugin->name() <<
        " returned invalid IPEndPoint: " << ep.ip() << ": " << ep.port();

      continue;
    }

    Log(INFO) << "Plugin " << plugin->name() << " will serve"
      " connections on " << ep.ip() << ": " << ep.port();

    SockType type = plugin->sock_type();
    switch (type) {
      case TCP: {
        std::shared_ptr<TcpListener> newSock(new TcpListener(ep));

        int err = 0;

        if (!newSock->Open(&err)) {
          Log(ERR) << "Socket wasn't opened " << plugin->name() << ": " << err;
          continue;
        }

        if (!newSock->Bind(&err)) {
          Log(ERR) << "Socket wasn't bound " <<
            plugin->name() << ": " << err;
          continue;
        }

        if (!newSock->Listen(m_maxListen, &err)) {
          Log(ERR) << "Socket isn't listening " <<
            plugin->name() << ": " << err;
          continue;
        }

        AcceptorSession* acceptorSession = new AcceptorSession(newSock, NULL,
          this, plugin);
        RegisterSession(acceptorSession);
      }
      break;

      // TODO(Olster): Add UDP support.
      default:
        Log(WARN) << "Unsupported socket type";
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
    int err = 0;
    int read = session->OnRead(&err);

    if (read < 1) {
      switch (read) {
        case 0:
          Log(INFO) << "Socket closed " << session->socket()->handle() <<
            ": " << err;
        break;

        case -1:
          Log(ERR) << "Socket error " << session->socket()->handle() <<
            ": " << err;
        break;

        default:
          Log(ERR) << "Socket error < -1 " << session->socket()->handle() <<
            ": " << err;
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
    int err = 0;
    int wrote = session->OnWrite(&err);
    if (wrote < 1) {
      switch (wrote) {
        case 0:
          Log(INFO) << "Socket sent 0 on write " <<
            session->socket()->handle() << ": " << err;
        break;

        case -1:
          Log(ERR) << "Socket error on write " <<
            session->socket()->handle() << ": " << err;
        break;

        default:
          Log(ERR) << "Socket error < -1 on write " <<
            session->socket()->handle() << ": " << err;
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
