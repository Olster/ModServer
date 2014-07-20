#ifndef SESSION_H_
#define SESSION_H_

#include <memory>

#include "base/build_required.h"

class Socket;
class ProtocolHandler;

class Session {
 public:
  Session(std::shared_ptr<Socket> sock,
          ProtocolHandler* protoHandler);
  virtual ~Session();

  virtual bool CanRead() = 0;
  virtual bool HasDataToSend() = 0;

  virtual int OnRead() = 0;
  virtual int OnWrite() = 0;
  virtual int OnError() = 0;

  std::shared_ptr<Socket> socket() { return m_sock; }
 protected:
  std::shared_ptr<Socket> m_sock;
  ProtocolHandler* m_protoHandler;

 private:
  DISALLOW_COPY_AND_ASSIGN(Session);
  DISALLOW_MOVE(Session);
};

#endif // SESSION_H_
