#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <string>
#include <map>

#include "base/build_required.h"

enum RequestMethod {
  GET = 0,
  INVALID
};

enum HttpVersion {
  HTTP_1_0 = 0,
  HTTP_1_1,
  INVALID
};

class HttpRequest {
 public:
  HttpRequest()
      : m_parsed(false),
        m_method(RequestMethod::INVALID),
        m_httpVer(HttpVersion::INVALID) {}

  bool Empty() const { return m_request.empty(); }
  void Clear();

  void Append(const std::string& data) { m_request.append(data); }
  void Append(const char* data, int size);

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
  bool m_parsed;

  std::string m_resPath;
  RequestMethod m_method;
  HttpVersion m_httpVer;

  DISALLOW_COPY_AND_ASSIGN(HttpRequest);
  DISALLOW_MOVE(HttpRequest);
};

class HttpRequestParser {
 public:
  enum ParseRes {
    OK = 0,
    INVALID_REQUEST,
    NOT_IMPLEMENTED,
    VERSION_NOT_SUPPORTED,
    MORE_DATA
  };

  static ParseRes Parse(HttpRequest& request);
  static RequestMethod MethodFromString(const std::string& method);
};

#endif // NET_HTTP_REQUEST_H_
