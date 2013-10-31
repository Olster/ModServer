#include "net/http_connection.h"
#include "net/socket/tcp_socket.h"

#ifdef DEBUG
#include <iostream>
// Not putting brackets around on purpose.
#define DEBUG_OUT(data) std::cout << data << std::endl;
#else
#define DEBUG_OUT(data)
#endif

#include <regex>

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
    return;
  }

  // GET / HTTP/1.1\r\n...
  static std::regex requestLineRegex(R"(^(GET|OPTIONS)\s+(\S+)\s+HTTP/(\d)\.(\d)[[:cntrl:]]{2}(.+))");

  std::smatch match;
  if (!std::regex_search(m_request, match, requestLineRegex)) {
    DEBUG_OUT("Invalid request");
  } else {
    DEBUG_OUT("Request:");
    DEBUG_OUT("Option: " << match[1].str());
    DEBUG_OUT("Path: " << match[2].str());
    DEBUG_OUT("HTTP major version: " << match[3].str());
    DEBUG_OUT("HTTP minor version: " << match[4].str() << '\n');
    DEBUG_OUT("Rest: " << match[5].str() << '\n');
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

  // Request has been processed, now clear it.
  m_request.clear();
}

int HttpConnection::SendResponse() {
  int bytesSend = m_clientSock->Send(m_response.c_str(), m_response.length());
  m_response.clear();

  return bytesSend;
}
} // namespace net
