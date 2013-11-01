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
#include <unordered_map>

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
  static std::regex requestLineRegex(R"(^(GET|OPTIONS)\s+(\S+)\s+HTTP/(\d)\.(\d)[[:cntrl:]]{2}.+)");

  std::smatch matcherResult;
  if (!std::regex_search(m_request, matcherResult, requestLineRegex)) {
    DEBUG_OUT("Invalid request");
    FormatInvalidRequestResponse();
  } else {
    DEBUG_OUT("Request:");
    DEBUG_OUT("Option: " << matcherResult[1].str());
    DEBUG_OUT("Path: " << matcherResult[2].str());
    DEBUG_OUT("HTTP major version: " << matcherResult[3].str());
    DEBUG_OUT("HTTP minor version: " << matcherResult[4].str() << '\n');
    DEBUG_OUT("Rest: " << matcherResult.format("$'") << '\n');

    // Current request method.
    Method method = MethodFromString(matcherResult[1].str());

    // TODO(Olster): Search for ".." exploit.
    std::string resourcePath = matcherResult[2].str();

    HttpVersion httpVer = HTTP_ERROR;

    // TODO(Olster): Maybe add function to detect the version.
    if ((matcherResult[3].str() == "1") && (matcherResult[4].str() == "1")) {
      httpVer = HTTP_1_1;
    }

    // Chop off the first request line.
    m_request = matcherResult.format("$'");

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
  }

  // Request has been processed, now clear it.
  m_request.clear();
}

int HttpConnection::SendResponse() {
  int bytesSend = m_clientSock->Send(m_response.c_str(), m_response.length());
  m_response.clear();

  return bytesSend;
}

// This new return feature helps to make code quite shorter!
auto HttpConnection::MethodFromString(const std::string& method) -> Method {
  static std::unordered_map<const std::string, Method> stringToMethodMap = {
    {"GET", Method::GET}
  };

  auto foundElem = stringToMethodMap.find(method);
  if (foundElem != stringToMethodMap.end()) {
    return foundElem->second;
  }

  // Error, no such method.
  return Method::METHOD_MAX;
}
} // namespace net
