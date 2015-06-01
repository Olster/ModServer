#include "server_plugin/http/request.h"

#include <cassert>

#include "plugin_api/plugin_log.h"

void HttpRequest::Clear() {
   m_request.clear();
}

void HttpRequest::Append(const char* data, int size) {
  Append(std::string(data, size));
}

// static
HttpRequestParser::ParseRes HttpRequestParser::Parse(HttpRequest& request) {
  PluginLog(INFO) << "Parsing\n" << request.data() << "END";

  size_t dataSize = request.data().size();
  if (dataSize > 0) {
    if (request.data().find("\r\n\r\n") != std::string::npos) {
      return HttpRequestParser::OK;
    }
  }

  return HttpRequestParser::MORE_DATA;
}

// NOTE(Olster): |method| must be uppercase string, otherwise INVALID_METHOD
// is returned.
RequestMethod HttpRequestParser::MethodFromString(const std::string& method) {
  // TODO(Olster): Use gperf (perfect hash) for lookup.
  if (method == "GET") {
    return GET;
  }

  // Error, no such method.
  return RequestMethod::INVALID_METHOD;
}
