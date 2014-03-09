#ifndef NET_HTTP_REQUEST_H_
#define NET_HTTP_REQUEST_H_

#include <string>
#include <map>

#include "base/build_required.h"

#include "net/http.h"

class HttpRequest {
 public:
  HttpRequest() {
    m_request.resize(m_maxBytesRead + 1);
  }

  DISALLOW_COPY_AND_ASSIGN(HttpRequest);
  DISALLOW_MOVE(HttpRequest);

  bool Empty() const { return m_request[0] == '\0'; }

  // Clears an internal buffer, retains buffer size.
  void Clear();

  size_t CanReadBytes() const { return m_maxBytesRead; }

  char* buffer() { return const_cast<char*>(m_request.c_str()); }
  size_t buffer_size() const { return m_request.size(); }

  const std::string& request() const { return m_request; }

  const std::string& resource_path() const { return m_resPath; }
  RequestMethod method() const { return m_method; }
  HttpVersion http_version() const { return m_httpVer; }

  void set_resource_path(const std::string& resPath) { m_resPath = resPath; }
  void set_request_method(const RequestMethod method) { m_method = method; }
  void set_http_ver(const HttpVersion ver) { m_httpVer = ver; }
 private:
  std::string m_request;
  bool m_parsed = false;

  std::string m_resPath;
  RequestMethod m_method = RequestMethod::INVALID_METHOD;
  HttpVersion m_httpVer = HttpVersion::HTTP_ERROR;

  // |4096| would be the default max block we can read in one go.
  static const size_t m_maxBytesRead = 4096;
};

class HttpRequestParser {
 public:
  enum ParseRes {
    OK = 0,
    INVALID_REQUEST,
    NOT_IMPLEMENTED,
    VERSION_NOT_SUPPORTED
  };

  static ParseRes Parse(HttpRequest& request);
  static RequestMethod MethodFromString(const std::string& method);
};

#endif // NET_HTTP_REQUEST_H_
