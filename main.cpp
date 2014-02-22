#include <cassert>

#include "net/http_server.h"
#include "net/ipendpoint.h"
#include "base/logger.h"

int main() {
  bool logStarted = Logger::InitLog();
  assert(logStarted);
  if (!logStarted) {
    printf("Log file wasn't opened.");
    return -1;
  }

  // TODO(Olster): Read settings either from command line or settings file.
  
  HttpServer server(IPEndPoint("127.0.0.1", 2563));
  if (!server.MapHostToLocalPath("/", "D:/HTML/Languages")) {
    Logger::Log("Didn't map host to local path");
  }

  if (server.Start() != HttpServer::SUCCESS) {
    Logger::Log("Server didn't start: %s", server.ErrorString().c_str());
    return -1;
  }
  
  // TODO(Olster): Will the server need stop functionality?
  while (true) {
    HttpServer::UpdateCode code = server.UpdateConnections();
    switch (code) {
      case HttpServer::TIME_OUT:
        continue;
      break;

      case HttpServer::UPDATE_ERROR:
        Logger::Log("Select entered error state.");
        return 2;
      break;
      
      default:
      break;
    }
    
    server.AcceptNewConnections();
    
    // Closes sockets to clients that have timed out, has closed the connection
    // or asked to close the connection.
    server.CloseUntrackedConnections();
        
    server.ReadRequests();
    server.ProcessRequests();
    server.SendResponses();
  }
  
  Logger::UninitLog();
  return 0;
}
