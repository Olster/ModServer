#ifndef SESSION_H_
#define SESSION_H_

#include "base/build_required.h"

class Socket;
class ProtocolHandler;

class Session {
 public:
  Session(Socket* sock, ProtocolHandler* protoHandler);
  virtual ~Session();

  virtual bool CanRead() = 0;
  virtual bool HasDataToSend() = 0;

  virtual int OnRead() = 0;
  virtual int OnWrite() = 0;
  virtual int OnError() = 0;

  Socket* socket() { return m_sock; }
 protected:
  Socket* m_sock;
  ProtocolHandler* m_protoHandler;

 private:
  DISALLOW_COPY_AND_ASSIGN(Session);
  DISALLOW_MOVE(Session);
};

#endif // SESSION_H_
