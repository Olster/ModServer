#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

#include "base/build_required.h"

class Header;

class HttpResponse {
 public:
  enum Status {
    OK,
    NOT_FOUND
  };

  HttpResponse() {}
  ~HttpResponse() {}

  const std::string& data() const { return m_data; }
  size_t size() const { return m_data.size(); }

  // Shifts data left by |bytes| bytes.
  // This is done when only the part of the message was sent.
  // TODO(Olster): Update this to move just the pointer.
  bool ShiftLeftBy(size_t bytes);

  void Clear() {
    m_data.clear();
  }

  bool HasData() const { return !m_data.empty(); }

  void SetStatus(Status status);
  void AddHeader(const Header& header);
  void SetContent(const std::string& content);
 private:
  std::string m_data;

  DISALLOW_COPY_AND_ASSIGN(HttpResponse);
  DISALLOW_MOVE(HttpResponse);
};

#endif // NET_HTTP_RESPONSE_H_
