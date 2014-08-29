#include "server_plugin/http/request.h"

#include <cassert>
#include <regex>

//#include "base/logger.h"

void HttpRequest::Clear() {
   m_request.clear();
}

void HttpRequest::Append(const char* data, int size) {
  Append(std::string(data, size));
}

// static
//HttpRequestParser::ParseRes HttpRequestParser::Parse(HttpRequest& request) {
//  // GET / HTTP/1.1\r\n...
//  static std::regex requestLineRegex(R"(^(GET|OPTIONS)\s+(\S+)\s+HTTP/(\d)\.(\d)[[:cntrl:]]{2}.+)");
//
//  std::smatch matcherResult;
//  if (!std::regex_search(request.data(), matcherResult, requestLineRegex)) {
//    //Log(WARN) << "Invalid request";
//    return INVALID_REQUEST;
//  }
//
//  //Log(INFO) << "Option: " << matcherResult[1].str();
//  //Log(INFO) << "Path: " << matcherResult[2].str();
//
//  std::string resourcePath = matcherResult[2].str();
//  if (resourcePath == "/") {
//    resourcePath += "index.html";
//  }
//
//  // Can't go back in path.
//  if (resourcePath.find("..") != std::string::npos) {
//    return INVALID_REQUEST;
//  }
//
//  request.set_resource_path(resourcePath);
//
//  // Current request method.
//  RequestMethod method = MethodFromString(matcherResult[1].str());
//  if (method == RequestMethod::INVALID_METHOD) {
//    return NOT_IMPLEMENTED;
//  }
//
//  request.set_request_method(method);
//
//  HttpVersion httpVer = INVALID_VERSION;
//
//  // TODO(Olster): Parse version with regex.
//  if ((matcherResult[3].str() == "1") && (matcherResult[4].str() == "1")) {
//    httpVer = HTTP_1_1;
//  }
//
//  if (httpVer == INVALID_VERSION) {
//    return VERSION_NOT_SUPPORTED;
//  }
//
//  request.set_http_ver(httpVer);
//
//  // TODO(Olster): Parse headers.
//
//  return OK;
//}

// static
HttpRequestParser::ParseRes HttpRequestParser::Parse(HttpRequest& request) {
  size_t dataSize = request.data().size();
  if (dataSize > 0) {
    if (request.data().find("\r\n\r\n") != std::string::npos) {
      return HttpRequestParser::OK;
    }
  }

  return HttpRequestParser::MORE_DATA;
}

RequestMethod HttpRequestParser::MethodFromString(const std::string& method) {
  static std::map<const std::string, RequestMethod> stringToMethodMap = {
    {"GET", RequestMethod::GET}
  };

  auto foundElem = stringToMethodMap.find(method);
  if (foundElem != stringToMethodMap.end()) {
    return foundElem->second;
  }

  // Error, no such method.
  return RequestMethod::INVALID_METHOD;
}
