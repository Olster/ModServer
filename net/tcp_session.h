#ifndef NET_TCP_SESSION_H_
#define NET_TCP_SESSION_H_

#include "net/session.h"

class Server;
class ServerPlugin;

class ConnectionSession : public Session {
 public:
  ConnectionSession(std::shared_ptr<Socket> sock, ProtocolHandler* protoHandler)
      : Session(sock, protoHandler) {}

  bool CanRead() override;
  bool HasDataToSend() override;

  int OnRead(int* err = NULL) override;
  int OnWrite(int* err = NULL) override;
  int OnError(int* err = NULL) override { return 0; }
};

class AcceptorSession : public Session {
 public:
  AcceptorSession(std::shared_ptr<Socket> sock,
                  ProtocolHandler* protoHandler,
                  Server* s,
                  ServerPlugin* plugin)
      : Session(sock, protoHandler),
        m_server(s), m_plugin(plugin) {}

  bool CanRead() override { return true; }
  bool HasDataToSend() override { return false; }

  int OnRead(int* err = NULL) override;
  int OnWrite(int* /*err*/) override { return 0; }
  int OnError(int* /*err*/) override { return 0; }

 private:
  // Not owning.
  Server* m_server;
  ServerPlugin* m_plugin;
};

#endif  // NET_TCP_SESSION_H_
