#include "net/http_connection.h"
#include "net/socket/tcp_socket.h"

namespace net {
HttpConnection::~HttpConnection()  {
  if (m_clientSock) {
    delete m_clientSock;
  }
}

int HttpConnection::ReadRequest() {
  char temp[4096];
  int read = m_clientSock->Receive(temp, sizeof(temp));

  if (read > 1) {
    temp[read - 1] = '\0';
    m_request = std::string(temp, read);
  }

  return read;
}

void HttpConnection::ProcessRequest() {
  if (m_request.empty()) {
    m_response.clear();
    return;
  }

  m_response = R"(<html>
                  <head>
                  	<title>Hello</title>
                  </head>
                  <body>
                    <h1>Testing server.</h1>
                  </body>
                  <html>)";

  int responeSize = m_response.length();
  m_response = "HTTP/1.1 200 OK\r\ncontent-type: text/html\r\ncontent-length: " +
               std::to_string(responeSize) +  "\r\n\r\n" + m_response;

  // Request has been processed, now clean it.
  m_request.clear();
}

int HttpConnection::SendResponse() {
  return m_clientSock->Send(m_response.c_str(), m_response.length());
}
} // namespace net
