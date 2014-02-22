#ifndef NET_SOCKET_HTTP_CONNECTION_H_
#define NET_SOCKET_HTTP_CONNECTION_H_

#include <string>
#include <map>
#include <cassert>

#include "base/build_required.h"
#include "resource/resource.h"

#include "net/http_request.h"

class TcpSocket;

class HttpConnection {
 public:
  DISALLOW_COPY_AND_ASSIGN(HttpConnection);
  DISALLOW_MOVE(HttpConnection);

  HttpConnection(TcpSocket* clientSock, const std::map<const std::string, const std::string>& hostToLocalPath)
   : m_clientSock(clientSock),
    m_hostToLocalPath(hostToLocalPath) {
    assert(m_clientSock);
  }

  ~HttpConnection();

  TcpSocket* clientSocket() const { return m_clientSock; }

  void SetReadyClose() { m_bReadyClose = true; }
  bool ReadyClose() const { return m_bReadyClose; }

  // Reads request from the client. Doesn't parse it. Returns number of bytes read.
  int ReadRequest();

  // Parces the request, forms a response.
  void ProcessRequest();

  // Sends response, cutting off sent data from actual response. Returns bytes sent.
  int SendResponse();

  bool DataAvailable() const { return !m_response.empty(); }
 private:
  // TODO(Olster): Add more method support.
  enum HttpMethod {
    GET = 0,
    INVALID_METHOD
  };

  Method MethodFromString(const std::string& method);

  // TODO(Olster): Eventually add support for HTTP/2.0.
  enum HttpVersion {
    HTTP_1_1 = 0,
    HTTP_ERROR
  };

  // Formats "400 Bad Request" message to send to client.
  void FormatBadRequestResponse();

  // Formats "404 Not Found" message.
  void FormatNotFoundResponse();

  // 501 Not Implemented
  void FormatNotImplementedResponse();

  TcpSocket* m_clientSock = nullptr;

  std::map<const std::string, const std::string> m_hostToLocalPath;

  bool m_bReadyClose = false;

  HttpRequest m_request;
  std::string m_response;

  Resource m_res;
  bool m_bAllResourceSent = true;

  // Bytes read from resource.
  size_t m_bytesRead = 0;
};

#endif // NET_SOCKET_HTTP_CONNECTION_H_
