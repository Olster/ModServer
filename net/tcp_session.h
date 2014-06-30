#ifndef TCP_SESSION_H_
#define TCP_SESSION_H_

#include "net/session.h"

class Server;
class ServerPlugin;

class ConnectionSession : public Session {
 public:
  ConnectionSession(Socket* sock, ProtocolHandler* protoHandler)
   : Session(sock, protoHandler) {}

  bool CanRead() override;
  bool HasDataToSend() override;

  int OnRead() override;
  int OnWrite() override;
  int OnError() override { return 0; }
};

class AcceptorSession : public Session {
 public:
  AcceptorSession(Socket* sock, ProtocolHandler* protoHandler, Server* s, ServerPlugin* plugin)
   : Session(sock, protoHandler),
     m_server(s), m_plugin(plugin) {}

  bool CanRead() override { return true; }
  bool HasDataToSend() override { return false; }

  int OnRead() override;
  int OnWrite() override { return 0; }
  int OnError() override { return 0; }

 private:
  Server* m_server;
  ServerPlugin* m_plugin;
};

#endif // TCP_SESSION_H_
