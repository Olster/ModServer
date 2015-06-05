#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <string>
#include <vector>

#include "base/build_required.h"

enum RequestMethod {
  GET = 0,
  INVALID_METHOD
};

enum HttpVersion {
  HTTP_1_0 = 0,
  HTTP_1_1,
  INVALID_VERSION
};

struct StringPiece {
  explicit StringPiece(const char* start = NULL, int len = 0)
    : m_start(start), m_length(len) {}

  std::string ToString() const;

  bool operator==(const StringPiece& other) const;

  const char* m_start;
  int m_length;
};

class HttpRequest;

class HttpRequestParser {
 public:
  enum ParseStatus {
    START = 0,
    METHOD,
    URL,
    HTTP,
    HEADER,
    HEADER_NEWLINE,
    FINISHED,
    MORE_DATA,
    ERROR
  };

  struct ParseRes {
    ParseRes(ParseStatus status, size_t stoppedAt) {
      this->status = status;
      this->stoppedAt = stoppedAt;
    }

    ParseStatus status;
    size_t stoppedAt;
  };

  static ParseRes Parse(HttpRequest& request);
};

class HttpRequest {
 public:
  HttpRequest()
      : m_parseRes(HttpRequestParser::START, 0),
        m_method(INVALID_METHOD),
        m_httpVer(INVALID_VERSION) {}

  bool Empty() const { return m_request.empty(); }
  void Clear();

  void Append(const std::string& data) { m_request.append(data); }
  void Append(const char* data, int size);

  void AddHeader(const StringPiece& header);

  const std::string& data() const { return m_request; }

  std::string resource_path() const { return m_resPath.ToString(); }
  RequestMethod method() const { return m_method; }
  HttpVersion http_version() const { return m_httpVer; }

  const std::vector<StringPiece>& headers() const { return m_headers; }

  void set_resource_path(const StringPiece& resPath) { m_resPath = resPath; }
  void set_request_method(const RequestMethod method) { m_method = method; }
  void set_http_ver(const HttpVersion ver) { m_httpVer = ver; }

  HttpRequestParser::ParseRes parse_res() const { return m_parseRes; }
  void set_parse_res(HttpRequestParser::ParseRes res) { m_parseRes = res; }
 private:
  std::string m_request;

  HttpRequestParser::ParseRes m_parseRes;

  StringPiece m_resPath;
  RequestMethod m_method;
  HttpVersion m_httpVer;

  std::vector<StringPiece> m_headers;

  DISALLOW_COPY_AND_ASSIGN(HttpRequest);
  DISALLOW_MOVE(HttpRequest);
};

#endif // NET_HTTP_REQUEST_H_
