#include "net/http_server.h"

HttpServer::HttpServer(const char* ip, unsigned short port, const std::string& resFolder)
 : m_listenerSocket(ip, port),
   m_resourcesFolderPath(resFolder) {

}

HttpServer::~HttpServer() {

}