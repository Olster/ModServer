#ifndef NET_HTTP_REQUEST_H_
#define NET_HTTP_REQUEST_H_

#include <string>

#include "base/build_required.h"

class HttpRequest {
 public:
  HttpRequest(const std::string& request = "")
   : m_request(request) {
    m_request.resize(m_maxBytesRead + 1);
  }

  DISALLOW_COPY_AND_ASSIGN(HttpRequest);
  DISALLOW_MOVE(HttpRequest);

  // TODO(Olster): Check if empty.
  bool Empty() const { return false; }

  // Clears an internal buffer. Issues a warning if there was unsent data there.
  void Clear();
  size_t CanReadBytes() const { return m_maxBytesRead; }

  void set_request(std::string&& req);
 private:
  std::string m_request;

  // |4096| would be the default max block we can read in one go.
  static const size_t m_maxBytesRead = 4096;
};

#endif // NET_HTTP_REQUEST_H_
