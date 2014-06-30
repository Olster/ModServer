#include "protocols/http/http_handler.h"

#include <cassert>

bool HttpHandler::HasDataToSend() const {
  return !m_response.empty();
}

void HttpHandler::DidReceive(char* data, int size) {
  std::string received(data, size);
  m_request += received;

  if (m_request.substr(m_request.length() - 4) == "\r\n\r\n") {
    std::string dataToSend = "<html><head><title>Hello</title></head><body><h1>Hello!</h1></body></html>";

    m_response = "HTTP/1.1 200 OK\r\n"
      "Content-length: " + std::to_string(dataToSend.length()) + "\r\n\r\n" +
      dataToSend;
  }
}

void HttpHandler::DidSend(int size) {
  assert(size > 0);
  if (size == m_response.length()) {
    m_response.clear();
  } else {
    m_response = m_response.substr(size);
  }
}

const char* HttpHandler::data_to_send() {
  if (!m_response.empty()) {
    return m_response.c_str();
  }
  
  return NULL;
}

size_t HttpHandler::data_to_send_size() {
  return m_response.length();
}
