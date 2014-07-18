#ifndef HTTP_H_
#define HTTP_H_

/*
// TODO(Olster): Add more method support.
// Request methods.
enum RequestMethod {
  GET = 0,
  INVALID_METHOD
};

// TODO(Olster): Eventually add support for HTTP/2.0.
enum HttpVersion {
  HTTP_1_1 = 0,
  HTTP_ERROR
};
*/

class ServerPlugin;

ServerPlugin* __declspec(dllexport) Instance();

#endif // NET_HTTP_H_
