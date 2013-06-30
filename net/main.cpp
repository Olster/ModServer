//
// This file is intended for testing putposes only
// currently it looks like a mess where I'm trying to figure out
// the concepts of HTTP protocol
//
#include <iostream>

#include "net/http_server.h"
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
//  if (argc < 4) {
//    cout << "Usage: exec_name port pending_connections file_path" << endl;
//    return -1;
//  }

  // OR
  // net::ServerConfig config;
  // config.setPort(2563).setPendingConnections(1).setFilePath("/home/olster/www");
  //
  //net::HTTPServer(config);

  // Port, pending connections, path to look for sites
  net::HTTPServer server(2563, 1, "/home/olster/cpp_progr/Server/bin/Debug");

  //net::HTTPServer server(std::stoi(argv[1]), std::stoi(argv[2]), argv[3]);

  if (server.IsInErrorState()) {
    cout << "Error: failed to start the server. Reason: " << server.GetErrorMsg() << endl;
    return -1;
  }

  while (true) {
    if (server.IsInErrorState()) {
      cout << "Server error: " << server.GetErrorMsg() << endl;
      break;
    }

    if (!server.UpdateSessions()) {
      cout << "Error: " << server.GetErrorMsg() << endl;
      break;
    }

    //server.CloseTimedOutSessions();
    server.AcceptNewConnections();

    server.ProcessSessions();

    // Don't hang up the CPU
    usleep(10000);
  }

  cin.ignore();

  return 0;
}
