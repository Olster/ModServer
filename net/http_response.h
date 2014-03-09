#ifndef NET_HTTP_RESPONSE_H_
#define NET_HTTP_RESPONSE_H_

#include <string>
#include <vector>

#include "base/build_required.h"
#include "net/http.h"

class HttpResponse {
 public:
  HttpResponse() = default;

  DISALLOW_COPY_AND_ASSIGN(HttpResponse);
  DISALLOW_MOVE(HttpResponse);

  // Formats full message with data and headers.
  const std::string& data() const { return m_data; }

  void set_data(std::string&& data) { m_data = std::move(data); }
  void set_data(const std::string& data) { m_data = data; }

  void Clear() { m_data.clear(); }

  // TODO(Olster): Set custom error messages.
  void NotImplemented501() {
    m_data = "HTTP/1.1 501 Not Implemented\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
  }

  void VersionNotSupported505() {
    m_data = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
  }

  void BadRequest400() {
    m_data = "HTTP/1.1 400 Bad Request\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
  }

  void NotFound404() {
    m_data = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-length: 0\r\n\r\n";
  }
 private:
  // Data along with headers.
  std::string m_data;
};

#endif // NET_HTTP_RESPONSE_H_
