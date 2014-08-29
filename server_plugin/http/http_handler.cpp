#include "server_plugin/http/http_handler.h"

#include <cassert>

bool HttpHandler::HasDataToSend() const {
  return !m_response.Empty();
}

void HttpHandler::DidReceive(char* data, int size) {
  m_request.Append(data, size);

  if (!HttpRequestParser::Parse(m_request) == HttpRequestParser::OK) {
    std::string dataToSend = "<html><head><title>Hello</title></head><body><h1>Hello!</h1></body></html>";

    m_response.set_data("HTTP/1.1 200 OK\r\n"
      "Content-length: " + std::to_string(dataToSend.length()) + "\r\n\r\n" +
      dataToSend);
  }
}

void HttpHandler::DidSend(int size) {
  assert(size > 0);
  if (size <= 0) {
    return;
  }

  if (static_cast<size_t>(size) >= m_response.length()) {
    m_response.Clear();
  } else {
    m_response.ShiftLeftBy(size);
  }
}

const char* HttpHandler::data_to_send() {
  if (!m_response.Empty()) {
    return m_response.data().c_str();
  }
  
  return NULL;
}

size_t HttpHandler::data_to_send_size() {
  return m_response.length();
}
