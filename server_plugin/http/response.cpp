#include "server_plugin/http/response.h"

#include "server_plugin/http/header.h"

#include <cassert>

namespace {
std::string GetStatusString(HttpResponse::Status status) {
  switch (status) {
    case HttpResponse::OK:
      return "HTTP/1.1 200 OK";
    break;

    case HttpResponse::NOT_FOUND:
      return "HTTP/1.1 404 Not Found";
    break;

    default:
    break;
  }

  return "HTTP/1.1 500 Internal Server Error";
}

const char* CRLF = "\r\n";
}  // namespace

bool HttpResponse::ShiftLeftBy(size_t bytes) {
  if (bytes > m_data.size()) {
    return false;
  }

  // Get the substring from |bytes| to the end of the data.
  m_data = m_data.substr(bytes);
  return true;
}

void HttpResponse::SetStatus(Status status) {
  m_data = GetStatusString(status);
  m_data += CRLF;
}

void HttpResponse::AddHeader(const Header& header) {
  m_data += header.ToString();
  m_data += CRLF;
}

void HttpResponse::SetContent(const std::string& content) {
  // Data must be preceded by empty line (CRLF).
  m_data += CRLF;

  m_data += content;

  // Content must end with 2 empty lines.
  m_data += CRLF;
  m_data += CRLF;
}
