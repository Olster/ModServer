#include <iostream>
#include "net/http_server.h"

using std::cout;
using std::endl;

<<<<<<< HEAD
int main() {
=======
int main(int argc, char* argv[]) {
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
  // TODO(Olster): Read settings either from command line or settings file.
  net::HttpServer server("127.0.0.1", 2563, "sitename.com");

  if (server.Start() != net::HttpServer::StartErrorCode::SUCCESS) {
    cout << "Server could not start: " << server.ErrorString() << endl;
    return 1;
  }
  
  // TODO(Olster): Will the server need stop functionality?
  while (true) {
    int selectRes = server.UpdateConnections();
    switch (selectRes) {
      // Select() timed out.
      case 0:
        continue;
      break;
      
      // Select() error.
      case -1:
        // TODO(Olster): Report error here (errno?).
        cout << "Select entered error state" << endl;
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
  
  return 0;
}
