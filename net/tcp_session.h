#ifndef NET_TCP_SESSION_H_
#define NET_TCP_SESSION_H_

#include "base/object.h"
#include "net/resource.h"
#include "net/http_request_parser.h"

namespace net {

class TCPSocket;

class TCPSession : public base::Object {
 public:
  TCPSession(net::TCPSocket* receiver, std::string resPath);
  ~TCPSession();

  DISALLOW_COPY_AND_ASSIGN(TCPSession);
  DISALLOW_MOVE(TCPSession);

  int Receive();
  int Send();

  TCPSocket* GetReceiver() { return m_receiver; }

  std::string ToString() const override { return "net::TCPSession."; }
 private:
  bool FillResource(const net::HttpRequestParser& parser, net::Resource& res);
  bool LoadResource(std::string filePath, net::Resource& res);

  TCPSocket* m_receiver = nullptr;
  std::string m_request;
  std::string m_response;
  net::Resource m_resource;
  net::HttpRequestParser m_requestParser;

  bool m_bAllDataSent = true;

  std::string m_notFoundResponse =
R"(<!DOCTYPE html>
<html>
<head>
  <title>It's not the page you might be looking for</title>
</head>
<body>
  <h3>Sorry, the page you requested is not available</h3>
</body>
</html>)";

  // Will load resources from here
  std::string m_resPath;
};

} // namespace net

#endif // NET_TCP_SESSION_H_
