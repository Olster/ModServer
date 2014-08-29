#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

#include "base/build_required.h"

class HttpResponse {
 public:
  HttpResponse() {}

  // Formats full message with data and headers.
  const std::string& data() const { return m_data; }

  // TODO(Olster): Critical. Update this function along with ShiftLeftBy().
  // Returns the amount of bytes that need to be sent.
  const size_t length() const { return m_data.length(); }

  void set_data(std::string&& data) { m_data = std::move(data); }
  void set_data(const std::string& data) { m_data = data; }

  // Shifts data left by |bytes| bytes.
  // This is done when only the part of the message was sent.
  bool ShiftLeftBy(size_t bytes);

  void Clear() { m_data.clear(); }

  bool Empty() const { return m_data.empty(); }

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

  DISALLOW_COPY_AND_ASSIGN(HttpResponse);
  DISALLOW_MOVE(HttpResponse);
};

#endif // NET_HTTP_RESPONSE_H_
