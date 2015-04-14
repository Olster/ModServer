#include "server_plugin/http/http_handler.h"

#include <cassert>

#include <fstream>

#include "plugin_api/data_chunk.h"
#include "plugin_api/plugin_log.h"

namespace {
const char* NotFoundPage() {
  return
    "<!doctype html>"
    "<html>"
    "<head>"
      "<title>Not found</title>"
    "</head>"
    "</html>";
}
}  // namespace

bool HttpHandler::HasDataToSend() const {
  return m_response.HasData();
}

void HttpHandler::DidReceive(DataChunk* data, int size) {
  // TODO(Olster): Make a list of data chunks rather than copying to a string.
  m_request.Append(data->buf_writable(), size);
  delete data;

  if (HttpRequestParser::Parse(m_request) == HttpRequestParser::OK) {
    std::string dataToSend;
    std::ifstream indexHtml;
    indexHtml.open("index.html");

    if (indexHtml.is_open()) {
      dataToSend = std::string(std::istreambuf_iterator<char>(indexHtml), std::istreambuf_iterator<char>());
    } else {
      dataToSend = NotFoundPage();
    }

    m_response.SetStatus(HttpResponse::OK);

    m_response.AddHeader(Header("Content-length", std::to_string(dataToSend.length())));
    m_response.AddHeader(Header("Connection", "keep-alive"));
    m_response.AddHeader(Header("Server", "PD Mod Server HTTP"));

    m_response.SetContent(dataToSend);

    PluginLog(INFO) << "Sending\n" << m_response.data();

    m_request.Clear();
  }
}

void HttpHandler::DidSend(int size) {
  assert(size > 0);
  if (size <= 0) {
    return;
  }

  if (static_cast<size_t>(size) >= m_response.size()) {
    m_response.Clear();
  } else {
    m_response.ShiftLeftBy(size);
  }
}

const char* HttpHandler::data_to_send() {
  if (m_response.HasData()) {
    return m_response.data().c_str();
  }

  return NULL;
}

size_t HttpHandler::data_to_send_size() {
  return m_response.size();
}

DataChunk* HttpHandler::AllocateChunk()  {
  return new DataChunk();
}

void HttpHandler::DiscardChunk(DataChunk* chunk) {
  delete chunk;
}
