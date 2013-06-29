//
// This file is intended for testing putposes only
// currently it looks like a mess where I'm trying to figure out
// the concepts of HTTP protocol
//

#include <iostream>

#include "net/server_socket.h"
#include "net/http_parser.h"
#include "net/resource.h"

// Temps
#include <error.h> // For perror()

using namespace std;

//net::Resource fillResource(const net::HTTPParser& parser, bool& success);
bool fillResource(const net::HTTPParser& parser, net::Resource& res);
bool loadResource(std::string filePath, net::Resource& res);

int main() {
  net::ServerSocket serverSock(2555);

  if (!serverSock.Open()) {
    cout << "Error: Couldn't open socket" << endl;
    return 1;
  }

  if (!serverSock.Bind()) {
    cout << "Error: Couldn't bind socket" << endl;
    return 2;
  }

  if (!serverSock.Listen(1)) {
    cout << "Error: failed to Listen()" << endl;
    return 3;
  }

  net::TCPSocket* recieverSock = nullptr;
  std::string request;
  std::string response;
  net::Resource resource;
  net::HTTPParser requestParser;

  bool allDataSent = true;

  std::string notFoundResponse =
R"(<!DOCTYPE html>
<html>
<head>
  <title>It's not the page you might be looking for</title>
</head>
<body>
  <h3>Sorry, the page you requested is not available</h3>
</body>
</html>)";

  int maxFileDescr = serverSock.GetHandle();

  fd_set masterSet;
  fd_set readerSet;
  fd_set writerSet;

  FD_ZERO(&masterSet);
  FD_ZERO(&readerSet);
  FD_ZERO(&writerSet);

  FD_SET(serverSock.GetHandle(), &masterSet);

  // Structure for select() timeout
  timeval timeout;
  timeout.tv_sec = 5*60;  // 5 mins
  timeout.tv_usec = 0;

  // Main loop of the server. Will loop until select() returns an error
  // (select() timeout doesn't break the loop).
  while (true) {
    // Copy master set to read and write set,
    // exclude serverSocket from writers set -- we won't write to ourselves
    readerSet = masterSet;
    writerSet = masterSet;

    FD_CLR(serverSock.GetHandle(), &writerSet);

    // Make a copy of timeout structure. Select() modifies it,
    // filling it with time left before timing out
    timeval timeLeft = timeout;

    int retCode = net::Socket::Select(maxFileDescr + 1, &readerSet, &writerSet, 0, &timeLeft);

    if (retCode == 0) {
      cout << "Info: select() timed out" << endl;
      continue;
    } else if (retCode == -1) {
      perror("Error: select() ");
      break;
    }

    // If serverSocket is ready to read, then it has pending connections
    // and Accept() won't block
    if (FD_ISSET(serverSock.GetHandle(), &readerSet)) {
#ifdef DEBUG
std::cout << "\tInfo: Accepting socket" << std::endl;
#endif
      if (recieverSock) {
        delete recieverSock;
        recieverSock = nullptr;
      }

      recieverSock = serverSock.Accept();

      if (recieverSock->GetHandle() > maxFileDescr) {
        maxFileDescr = recieverSock->GetHandle();
      }

      FD_SET(recieverSock->GetHandle(), &masterSet);
    }

    if (recieverSock) {
      // Receive
      if (FD_ISSET(recieverSock->GetHandle(), &readerSet)) {
        if (allDataSent) {
          int bytesReceived = recieverSock->Receive(request);
          if (bytesReceived == 0) {
            // Connection closed
#ifdef DEBUG
std::cout << "\tInfo: Socket was closed" << std::endl;
#endif
            FD_CLR(recieverSock->GetHandle(), &masterSet);

            delete recieverSock;
            recieverSock = nullptr;

            continue;
          }

          if (bytesReceived == -1) {
            // Socket error
#ifdef DEBUG
std::cout << "\tError: Connection error" << std::endl;
#endif
            delete recieverSock;
            recieverSock = nullptr;

            continue;
          }

#ifdef DEBUG
std::cout << "Received:\n" << request << std::endl;
#endif

          requestParser.Parse(request);

          if (requestParser.IsRequestValid()) {
            // If resource doesn't exist, give the 404 error
            bool resLoaded = fillResource(requestParser, resource);

            if (!resLoaded) {
              response = "HTTP/1.1 404 Not Found\nContent-type: text/html\nContent-length: " +
                         std::to_string(notFoundResponse.length()) + "\n\n" +
                         notFoundResponse + "\n\n";
            } else {
              response = "HTTP/1.1 200 OK\nContent-Type: " +
                          resource.GetMIMEType() + "\nContent-Length: " +
                          std::to_string(resource.GetData().length()) +
                          "\n\n" + resource.GetData() + "\n\n";
            }
          }
        }
        // Can't receive if not all data had been sent
      }

      // Send
      if (FD_ISSET(recieverSock->GetHandle(), &writerSet)) {
        // If we have nothing to send, then go on
        if (!response.empty()) {
#ifdef DEBUG
std::cout << "\tSending\n" << response << std::endl;
#endif

          int bytesSent = recieverSock->Send(response);
          if (bytesSent >= response.length()) {
            allDataSent = true;
            response.clear();
          } else {
            response = response.substr(bytesSent);
            allDataSent = false;
          }
        }
      }
    }
  }

  return 0;
}

bool fillResource(const net::HTTPParser& parser, net::Resource& res) {
  if (parser.GetResourceURI() == "/") {
    // Asking for the site root
    return loadResource("index.html", res);
  }

  return loadResource(parser.GetResourceURI(), res);
}

bool loadResource(std::string filePath, net::Resource& res) {
  // Should return error code

  // TODO(Olster): This is not reliable, would fail if the
  // requested file is too big. Use chunked transfer encoding?

  FILE* file = nullptr;

  // TODO(Olster): The path to the folder containing resources will be set
  // when creating the HTTPServer as a parameter in a constructor.
  // Or make a configuration class and pass it as a parameter.

  //filePath = "/home/olster/cpp_progr/Server/bin/Debug/" + filePath;

#ifdef DEBUG
  cout << "\tOpening file " << filePath << endl;
#endif

  file = fopen(filePath.c_str(), "rb");

  if (!file) {
    return false;
  }

  char* buf = nullptr;

  fseek(file, 0, SEEK_END);
  int size = ftell(file);

  fseek(file, 0, SEEK_SET);

  buf = new char[size + 1];
  buf[size] = '\0';

  // Read EVERYTHING in one go: read chunk of the size |size|
  // once.
  // TODO(Olster): Check for error code after reading
  fread(buf, size, 1, file);

  fclose(file);
  file = nullptr;

  // NOTE(Olster): Can't do the assignment, cause binary data would be corrupt
  // the constructor that takes char as a parameter will delete all non-printed
  // characters
  res.SetData(string(buf, size - 1));

  delete [] buf;
  buf = nullptr;

  string::size_type extensionDot = filePath.find_last_of('.');
  string extension = filePath.substr(extensionDot + 1);

#ifdef DEBUG
cout << "\tInfo: Resource extension: " << extension << endl;
#endif

  // TODO(Olster): Build a map that returns MIME type instead of ifs

  // Text
  if (extension == "html" || extension == "htm") {
    res.SetMIMEType("text/html");
  }

  if (extension == "css") {
    res.SetMIMEType("text/css");
  }

  // Image
  if (extension == "ico ") {
    res.SetMIMEType("image/x-image");
  }

  if (extension == "png") {
    res.SetMIMEType("image/png");
  }

  if (extension == "gif") {
    res.SetMIMEType("image/gif");
  }

  if (extension == "jpg" || extension == "jpeg") {
    res.SetMIMEType("image/jpeg");
  }

  return true;
}
