#include "net/http_connection.h"

#include <regex>
#include <map>

#include "net/socket/tcp_socket.h"
#include "base/logger.h"

HttpConnection::~HttpConnection()  {
  if (m_clientSock) {
    delete m_clientSock;
  }
}

int HttpConnection::ReadRequest() {
  m_request.Clear();

  char buff[4096];
  int read = m_clientSock->Receive(buff, ARR_SIZE(buff));

  if (read > 0) {
    buff[read + 1] = '\0';
    m_request.set_request(std::string(buff, read));
  }

  return read;
}

// TODO(Olster): Add information about server when formatting response.
// Check if the request is full.
void HttpConnection::ProcessRequest() {
  if (m_request.Empty() && m_bAllResourceSent) {
    return;
  }

  if (m_bAllResourceSent) {
    bool parsed = HttpRequestParser::Parse(m_request);
    if (parsed) {
      std::string resPath = m_request.resource_path();
      HttpMethod method = m_request.method();
      HttpVersion httpVer = m_request.http_version();
    } else {
      FormatBadRequestResponse();
    }
  }

  

  if (m_bAllResourceSent) {
    // GET / HTTP/1.1\r\n...
    static std::regex requestLineRegex(R"(^(GET|OPTIONS)\s+(\S+)\s+HTTP/(\d)\.(\d)[[:cntrl:]]{2}.+)");

    std::smatch matcherResult;
    if (!std::regex_search(m_request, matcherResult, requestLineRegex)) {
      Logger::Log("Invalid request");
      FormatBadRequestResponse();

      return;
    }

    Logger::Log("Request:");
    Logger::Log("Option: %s\nPath: %s\n", matcherResult[1].str().c_str(),
                matcherResult[2].str().c_str());

    // TODO(Olster): Search for ".." exploit.
    resourcePath = matcherResult[2].str();
    if (resourcePath == "/") {
      resourcePath += "index.html";
    }

    // Current request method.
    Method method = MethodFromString(matcherResult[1].str());
    if (method == Method::INVALID_METHOD) {
      FormatNotImplementedResponse();
      return;
    }
  
    // TODO(Olster): Maybe add function to detect the version.
    if ((matcherResult[3].str() == "1") && (matcherResult[4].str() == "1")) {
      httpVer = HTTP_1_1;
    }

    if (httpVer == HTTP_ERROR) {
      // TODO(Olster): Use 505 HTTP Version Not Supported.
      FormatBadRequestResponse();
      return;
    }

    // Chop off the first request line for further parsing.
    //m_request = matcherResult.format("$'");
  }

  // If set to false, no "HTTP/1.1 200 OK"... stuff would be generated.
  bool addHeader = true;

  // Open resource file if not yet opened.
  if (!m_bAllResourceSent) {
    // TODO(Olster): Track if we have a byte range header.
    assert(m_res.Opened());
    addHeader = false;
  } else {

    bool opened = false;

    // TODO(Olster): Read host from headers.
    auto it = m_hostToLocalPath.find("/");    
    if (it != m_hostToLocalPath.end()) {
      opened = m_res.Open(it->second + resourcePath);
    }
    
    if (!opened) {
      FormatNotFoundResponse();
      return;
    }
  }
  
  long fileSize = m_res.ResourceSizeBytes();
  assert(fileSize > 0);

  std::string data;
  m_bytesRead += m_res.Read(data, fileSize, m_bytesRead);
  if (m_bytesRead == static_cast<size_t>(fileSize)) {
    m_res.Close();

    // If all resource sent, set to 0;
    m_bytesRead = 0;
    m_bAllResourceSent = true;
  } else {
    m_bAllResourceSent = false;
  }

  // TODO(Olster): Add FormatOKResponse() function for consistency.
  if (addHeader) {
    m_response = "HTTP/1.1 200 OK\r\ncontent-type: " + m_res.MimeType() + "\r\ncontent-length: " +
                 std::to_string(fileSize) + "\r\n\r\n" + data;
  } else {
    m_response = std::move(data);
  }
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
