#include "server_plugin/http/request.h"

#include <cassert>

#include "plugin_api/plugin_log.h"

std::string StringPiece::ToString() const {
  if (!m_length || !m_start) {
    return std::string();
  }

  std::string out;
  out.resize(m_length);

  for (int i = 0; i < m_length; i++) {
    out[i] = m_start[i];
  }

  return out;
}

bool StringPiece::operator==(const StringPiece& other) const {
  if (other.m_length != m_length) {
    return false;
  }

  for (int i = 0; i < m_length; i++) {
    if (other.m_start[i] != m_start[i]) {
      return false;
    }
  }

  return true;
}

void HttpRequest::Clear() {
  m_request.clear();
}

void HttpRequest::Append(const char* data, int size) {
  Append(std::string(data, size));
}

void HttpRequest::AddHeader(const StringPiece& header) {
  m_headers.push_back(header);
}

namespace {
char getChar(const std::string& request, size_t currPos) {
  if (currPos >= request.length()) {
    return '\0';
  }

  return request[currPos];
}

RequestMethod MethodFromStringPiece(const StringPiece& methodString) {
  if (methodString == StringPiece("GET", 3)) {
    return RequestMethod::GET;
  }

  return RequestMethod::INVALID_METHOD;
}

HttpVersion HttpVerFromStringPiece(const StringPiece& httpString) {
  if (httpString == StringPiece("HTTP/1.1", 8)) {
    return HttpVersion::HTTP_1_1;
  } else if (httpString == StringPiece("HTTP/1.0", 8)) {
    return HttpVersion::HTTP_1_0;
  }

  return HttpVersion::INVALID_VERSION;
}
}  // namespace

// static
HttpRequestParser::ParseRes HttpRequestParser::Parse(HttpRequest& request) {
  //PluginLog(INFO) << "Parsing\n" << request.data() << "END";

  HttpRequestParser::ParseRes previousParseRes = request.parse_res();

  HttpRequestParser::ParseStatus status = previousParseRes.status;
  size_t currentPos = previousParseRes.stoppedAt;

  const std::string& requestString = request.data();
  const char* start = requestString.c_str();

  // Oh-oh, we're outside of bounds. Return error.
  if (currentPos >= requestString.size()) {
    return HttpRequestParser::ParseRes(HttpRequestParser::ERROR, currentPos);
  }

  // Advance to needed position.
  start += currentPos;

  StringPiece method;
  StringPiece url;
  StringPiece http;

  size_t pieceLen = 0;

  while ((status != ERROR) && (status != FINISHED)) {
    switch (status) {
      case START:
      start = requestString.c_str();
      status = METHOD;
      break;

      case METHOD: {
        char nextChar = getChar(requestString, currentPos);

        switch (nextChar) {
          case '\0':
            status = FINISHED;
          break;

          case ' ':
            method.m_start = start;
            method.m_length = pieceLen;

            currentPos++;

            start = &requestString[currentPos];
            pieceLen = 0;

            status = URL;
          break;

          default:
            pieceLen++;
            currentPos++;
          break;
        }
      }
      break;

      case URL: {
        char nextChar = getChar(requestString, currentPos);

        switch (nextChar) {
          case '\0':
            status = FINISHED;
          break;

          case ' ':
            url.m_start = start;
            url.m_length = pieceLen;

            currentPos++;

            start = &requestString[currentPos];
            pieceLen = 0;

            status = HTTP;
          break;

          default:
            pieceLen++;
            currentPos++;
          break;
        }
      }
      break;

      case HTTP: {
        char nextChar = getChar(requestString, currentPos);

        switch (nextChar) {
          case '\0':
            status = FINISHED;
          break;

          case '\r':
            currentPos++;
          break;

          case '\n':
            http.m_start = start;
            http.m_length = pieceLen;

            currentPos++;

            start = &requestString[currentPos];
            pieceLen = 0;

            status = HEADER;
          break;

          default:
            pieceLen++;
            currentPos++;
          break;
        }
      }
      break;

      case HEADER: {
        char nextChar = getChar(requestString, currentPos);

        switch (nextChar) {
          case '\0':
            status = FINISHED;
          break;

          case '\r':
            currentPos++;
          break;

          case '\n': {
            StringPiece header;
            header.m_start = start;
            header.m_length = pieceLen;

            request.AddHeader(header);

            currentPos++;

            start = &requestString[currentPos];
            pieceLen = 0;

            status = HEADER_NEWLINE;
          }
          break;

          default:
            pieceLen++;
            currentPos++;
          break;
        }
      }
      break;

      case HEADER_NEWLINE: {
        char nextChar = getChar(requestString, currentPos);

        switch (nextChar) {
          case '\r':
            currentPos++;
          break;

          case '\n':
            status = FINISHED;
          break;

          default:
            status = HEADER;
          break;
        }
      }
      break;

      default:
        status = MORE_DATA;
      break;
    }
  }

  HttpRequestParser::ParseRes out(status, currentPos);

  request.set_request_method(MethodFromStringPiece(method));
  request.set_resource_path(url);
  request.set_http_ver(HttpVerFromStringPiece(http));
  request.set_parse_res(out);

  return out;
}
