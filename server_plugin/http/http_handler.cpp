#include "server_plugin/http/http_handler.h"

#include <cassert>

#include <fstream>

#include "plugin_api/data_chunk.h"
#include "plugin_api/plugin_log.h"

#include "server_plugin/http/request.h"

namespace {
std::string NotFoundPage() {
  std::ifstream notFoundPage;
  notFoundPage.open("404_page.html");

  std::string out;
  if (notFoundPage.is_open()) {
    out = std::string(std::istreambuf_iterator<char>(notFoundPage), std::istreambuf_iterator<char>());
  } else {
    out =
    "<!doctype html>"
    "<html>"
    "<head>"
      "<title>Not found</title>"
      "<body><p>404 Not Found</p></body>"
    "</head>"
    "</html>";
  }

  return out;
}
}  // namespace

bool HttpHandler::HasDataToSend() const {
  return m_response.HasData();
}

#include <windows.h>

void HttpHandler::DidReceive(DataChunk* data, int size) {
  DWORD timeStart = ::GetTickCount();

  if (!m_request) {
    m_request = new HttpRequest();
  }

  // TODO(Olster): Make a list of data chunks rather than copying to a string.
  m_request->Append(data->buf_writable(), size);
  delete data;

  if (HttpRequestParser::Parse(*m_request).status == HttpRequestParser::FINISHED) {
    PluginLog(INFO) << "Method: " << m_request->method();
    PluginLog(INFO) << "URL: " << m_request->resource_path();
    PluginLog(INFO) << "HTTP: " << m_request->http_version();

    for (const auto& header : m_request->headers()) {
      PluginLog(INFO) << header.ToString();
    }

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

    //PluginLog(INFO) << "Sending\n" << m_response.data();
    delete m_request;
    m_request = NULL;
  }

  PluginLog(INFO) << "Parsing took: " << ::GetTickCount() - timeStart;
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
