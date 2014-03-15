#include "net/http_connection.h"


#include <map>

#include "net/socket/tcp_socket.h"
#include "net/http.h"

#include "base/logger.h"

HttpConnection::~HttpConnection()  {
  if (m_clientSock) {
    delete m_clientSock;
  }
}

int HttpConnection::ReadRequest() {
  m_request.Clear();
  return m_clientSock->Receive(m_request.buffer(), m_request.buffer_size());
}

// TODO(Olster): Add information about server when formatting response.
// Check if the request is full.
void HttpConnection::ProcessRequest() {
  if (m_request.Empty() && m_bAllResourceSent) {
    return;
  }

  std::string resPath = "";
  RequestMethod method = INVALID_METHOD;
  HttpVersion httpVer = HTTP_ERROR;

  if (m_bAllResourceSent) {
    HttpRequestParser::ParseRes res = HttpRequestParser::Parse(m_request);

    switch (res) {
      case HttpRequestParser::OK:
        resPath = m_request.resource_path();
        method = m_request.method();
        httpVer = m_request.http_version();
      break;

      case HttpRequestParser::NOT_IMPLEMENTED:
        m_response.NotImplemented501();
      break;

      case HttpRequestParser::VERSION_NOT_SUPPORTED:
        m_response.VersionNotSupported505();
      break;

      default:
      case HttpRequestParser::INVALID_REQUEST:
        m_response.BadRequest400();
      break;
    }
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
      opened = m_res.Open(it->second + resPath);
    }

    if (!opened) {
      m_response.NotFound404();
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
    m_response.set_data("HTTP/1.1 200 OK\r\ncontent-type: " + m_res.MimeType() + "\r\ncontent-length: " +
                 std::to_string(fileSize) + "\r\n\r\n" + data);
  } else {
    m_response.set_data(data);
  }
}

int HttpConnection::SendResponse() {
  int bytesSend = m_clientSock->Send(m_response.data().c_str(), m_response.data().length());

  if (bytesSend != static_cast<int>(m_response.data().length())) {
    Logger::Log("Partial send happened, not implemented");
    assert(false && "Partial send happened");
  } else {
    m_response.Clear();
  }

  return bytesSend;
}
