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
#include <map>

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

// TODO(Olster): Add information about server when formatting response.
void HttpConnection::ProcessRequest() {
  if (m_request.empty()) {
    return;
  }

  // GET / HTTP/1.1\r\n...
  static std::regex requestLineRegex(R"(^(GET|OPTIONS)\s+(\S+)\s+HTTP/(\d)\.(\d)[[:cntrl:]]{2}.+)");

  std::smatch matcherResult;
  if (!std::regex_search(m_request, matcherResult, requestLineRegex)) {
    DEBUG_OUT("Invalid request");
    FormatBadRequestResponse();
  } else {
    DEBUG_OUT("Request:");
    DEBUG_OUT("Option: " << matcherResult[1].str());
    DEBUG_OUT("Path: " << matcherResult[2].str());
    DEBUG_OUT("HTTP major version: " << matcherResult[3].str());
    DEBUG_OUT("HTTP minor version: " << matcherResult[4].str() << '\n');
    DEBUG_OUT("Rest: " << matcherResult.format("$'") << '\n');

    do {
      // Current request method.
      Method method = MethodFromString(matcherResult[1].str());
      if (method == Method::INVALID_METHOD) {
        FormatNotImplementedResponse();
        break;
      }

      // TODO(Olster): Search for ".." exploit.
      std::string resourcePath = matcherResult[2].str();
      if (resourcePath == "/") {
        resourcePath += "index.html";
      }

      HttpVersion httpVer = HTTP_ERROR;
    
      // TODO(Olster): Maybe add function to detect the version.
      if ((matcherResult[3].str() == "1") && (matcherResult[4].str() == "1")) {
        httpVer = HTTP_1_1;
      }

      if (httpVer == HTTP_ERROR) {
        // TODO(Olster): Use 505 HTTP Version Not Supported.
        FormatBadRequestResponse();
        break;
      }

      // Chop off the first request line for further parsing.
      //m_request = matcherResult.format("$'");

      // Open resource file.
      if (m_res.Open(m_files + resourcePath)) {
        long fileSize = m_res.ResourceSizeBytes();

        // Assert that files is less than ~60kb.
        // TODO(Olster): Figure out how to send partial responses.
        assert(fileSize < 60000);
        m_res.Read(m_response, fileSize);

        // TODO(Olster): Add FormatOKResponse() function for consistency.
        m_response = "HTTP/1.1 200 OK\r\ncontent-type: " + m_res.MimeType() + "\r\ncontent-length: " +
                     std::to_string(fileSize) + "\r\n\r\n" + m_response;
      } else {
        FormatNotFoundResponse();
        break;
      }
    } while (false);
  }

  m_request.clear();
}

int HttpConnection::SendResponse() {
  int bytesSend = m_clientSock->Send(m_response.c_str(), m_response.length());
  
  if (bytesSend != m_response.length()) {
    // Partial send.
    m_response = m_response.substr(bytesSend);
  } else {
    m_response.clear();
  }

  return bytesSend;
}

// This new return feature helps to make code quite shorter!
auto HttpConnection::MethodFromString(const std::string& method) -> Method {
  static std::map<const std::string, Method> stringToMethodMap = {
    {"GET", Method::GET}
  };

  auto foundElem = stringToMethodMap.find(method);
  if (foundElem != stringToMethodMap.end()) {
    return foundElem->second;
  }

  // Error, no such method.
  return Method::INVALID_METHOD;
}

void HttpConnection::FormatBadRequestResponse() {
  m_request = "HTTP/1.1 400 Bad Request\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
}

void HttpConnection::FormatNotFoundResponse() {
  m_request = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
}

void HttpConnection::FormatNotImplementedResponse() {
  m_request = "HTTP/1.1 501 Not Implemented\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
}
} // namespace net
