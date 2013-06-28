#ifndef NET_HTTPPARSER_H
#define NET_HTTPPARSER_H

#include <string>

#include "base/object.h"

// TODO(Olster): This is a HTTPRequestParser
// make an abstract HTTPParser and Request and Response parsers

namespace net {

class HTTPParser : public base::Object {
 public:

  enum RequestMethod {
    GET,
    HEAD,
    ERROR
  };

  enum class ParserState {
    NOT_PARSED,
    PARSED,
    ERROR
  };

  enum class HTTPVersion {
    VER_1_0,
    VER_1_1,
    VER_CUSTOM
  };

  HTTPParser() = default;
  ~HTTPParser() = default;

  void operator=(const HTTPParser& other) = delete;
  HTTPParser(const HTTPParser& other) = delete;

  void operator=(HTTPParser&& other) = delete;
  HTTPParser(HTTPParser&& other) = delete;

  ParserState Parse(std::string& request);

  // Sets parser into initial state
  void Reset();

  ParserState GetState() const { return m_internalState; }

  RequestMethod GetRequestMethod() const { return m_reqMethod; }

  std::string GetResourceURI() const { return m_resourceURI; }

  HTTPVersion GetHTTPVersion() const { return m_httpVer; }

  int GetMajHTTPVersion() const { return m_dHTTPMajVer; }
  int GetMinHTTPVersion() const { return m_dHTTPMinVer; }

  int GetErrorPos() const { return m_dErrorPos; }

  std::string ToString() const override { return "net::HTTPParser."; }

 private:
  // Chops off request line grom request in the process
  ParserState ParseRequestLine(std::string& request);

  ParserState ParseHeaders(std::string& request);

  ParserState m_internalState = ParserState::NOT_PARSED;

  RequestMethod m_reqMethod = RequestMethod::GET;
  std::string m_resourceURI;

  HTTPVersion m_httpVer = HTTPVersion::VER_CUSTOM;

  int m_dHTTPMajVer = 0;
  int m_dHTTPMinVer = 0;

  // Error position in the request
  int m_dErrorPos = -1;
};

} // namespace net

#endif // NET_HTTPPARSER_H
