#include "net/server.h"
#include "net/ip_endpoint.h"
#include "base/logger.h"
#include "protocol_factory/http_protocol_factory.h"

struct LogAutoUninit {
  ~LogAutoUninit() {
    Logger::UninitLog();
  }
};

int main(int argc, char** argv) {
  if (argc < 4) {

    // TODO(Olster): Make a command line parser.
    // Passing params without flags for now.
    printf("Usage: exeName ip port path. For example,"
           "sthttps.exe /addr:127.0.0.1 /port:2563 /host:D:/SiteFolder");
    return 0;
  }

  if (!Logger::InitLog()) {
    printf("Log file wasn't opened.");
    return 0;
  }

  LogAutoUninit logUninit;

  std::string ip = argv[1];
  unsigned short port = static_cast<unsigned short>(strtoul(argv[2], NULL, 10));
  std::string path = argv[3];

  // TODO(Olster): Read settings from settings file.

  Server httpServer(IPEndPoint(ip, port), new HttpProtocolFactory);
  Server::StartErrorCode rc = httpServer.Start();

  // TODO(Olster): Get actual underlying error codes.
  switch (rc) {
    case Server::SOCKET_NOT_OPENED:
      Logger::Log("Socket wasn't opened");
      return 0;
    break;

    case Server::SOCKET_NOT_BOUND:
      Logger::Log("Socket wasn't bound");
      return 0;
    break;

    case Server::SOCKET_NOT_LISTENING:
      Logger::Log("Socket isn't listening");
      return 0;
    break;

    case Server::SUCCESS:
    default:
    break;
  }
  
  while (!httpServer.Stopped()) {
    Server::UpdateCode code = httpServer.UpdateConnections();
    switch (code) {
      case Server::TIME_OUT:
        continue;
      break;

      case Server::UPDATE_ERROR:
        Logger::Log("Select entered error state.");
        httpServer.Shutdown();
      break;
      
      default:
      break;
    }
    
    httpServer.AcceptNewConnections();        
    httpServer.ReadRequests();
    httpServer.SendResponses();
  }
  
  return 0;
}
