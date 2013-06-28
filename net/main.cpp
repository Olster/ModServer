//
// This file is intended for testing putposes only
// currently it looks like a mess where I'm trying to figure out
// the concepts of HTTP protocol
//

#include <iostream>

#include "net/server_socket.h"
#include "net/http_parser.h"

// Temps
#include <error.h> // For perror()

using namespace std;

// TODO(Olster): Make a separate class or move to another file
struct Resource {
  string path;
  string MIMEtype;

  string data;
};

Resource resourceRequested(const string& pathToRes);
void getResource(Resource& res);

int main() {
  net::ServerSocket httpServerSocket(2563);
  httpServerSocket.Open();
  httpServerSocket.Bind();
  httpServerSocket.Listen(5);

  // Everything below is just testing.

  fd_set master;
  fd_set readList;
  fd_set writeList;

  FD_ZERO(&master);
  FD_ZERO(&readList);
  FD_ZERO(&writeList);

  FD_SET(httpServerSocket.GetHandle(), &master);

  int maxFd = httpServerSocket.GetHandle();

  net::TCPSocket* sock = nullptr;

  bool bAnsverAvailable = false;

  // 5 min timeout structure
  timeval tv;
  tv.tv_sec = 5 * 60;
  tv.tv_usec = 0l;

  string dataReceived;
  net::HTTPParser requestParser;

  while (true) {
    timeval timeLeft = tv;

    readList = master;
    writeList = master;

    // We don't want to write to ourselves
    FD_CLR(httpServerSocket.GetHandle(), &writeList);

    int retCode = net::Socket::Select(maxFd + 1, &readList, &writeList, NULL, &timeLeft);

    if (retCode == 0) {
      cout << "Info: Timeout reached" << endl;

      // Closing timed out connection
      // by deleting socket for now
      if (sock) {
        FD_CLR(sock->GetHandle(), &master);
        delete sock;
        sock = nullptr;
      }

      continue;
    } else if (retCode == -1) {
      perror("An error has occured: ");
      break;
    }

    if (FD_ISSET(httpServerSocket.GetHandle(), &readList)) {
      cout << "Info: Accepting socket" << endl;
      sock = httpServerSocket.Accept();

      net::InternalSockType sockFileDescr = sock->GetHandle();

      if (sockFileDescr > maxFd) {
        maxFd = sockFileDescr;
      }

      FD_SET(sockFileDescr, &master);
    }

    if (sock) {
      if (FD_ISSET(sock->GetHandle(), &readList)) {
        cout << "Info: Receiving from " << sock->GetHandle() << endl;

        int readSize = sock->Receive(dataReceived);

        if (readSize > 0) {
          cout << dataReceived << endl;

          net::HTTPParser::ParserState state = requestParser.Parse(dataReceived);

          if (state == net::HTTPParser::ParserState::PARSED) {
            bAnsverAvailable = true;
          }
        } else if (readSize == 0) {
          cout << "Info: Socket " << sock->GetHandle() << " closed the connection" << endl;
          FD_CLR(sock->GetHandle(), &master);
          dataReceived.clear();

          delete sock;
          sock = nullptr;

          bAnsverAvailable = false;
        } else {
          cout << "Error: Socket error" << endl;
        }
      }
    } else {
      cout << "Info: sock is NULL" << endl;
    }

    if (sock) {
      if (FD_ISSET(sock->GetHandle(), &writeList) && bAnsverAvailable) {
        cout << "Info: Sending data to " << sock->GetHandle() << endl;

        Resource res {"", "", ""};

        if (requestParser.GetState() == net::HTTPParser::ParserState::PARSED) {
          res = resourceRequested(requestParser.GetResourceURI());
          getResource(res);
        }

        if (sock) {
          string formatted = "";
          if (!res.MIMEtype.empty()) {
             formatted = "HTTP/1.1 200 OK\nContent-Type: " + res.MIMEtype + "\nContent-Length: " + std::to_string(res.data.length()) + "\n\n" + res.data + "\n\n";
          } else {
            formatted = "HTTP/1.1 404 Not Found\n\n";
          }

  #ifdef DEBUG
          cout << "\nSending: " << formatted << endl << endl;
  #endif

          sock->Send(formatted);
          cout << "Info: Data sent" << endl;
        } else {
          cout << "Socket was closed" << endl;
        }

        //FD_CLR(acceptedSock, &master);

        bAnsverAvailable = false;
      } else {
        //cout << "Not sending" << endl;
      }
    } else {
      cout << "Info: sock is NULL" << endl;
    }

    // Don't think I need sleep, select() acts as a sleep I presume
    //sleep(0);
  }

  if (sock) {
    delete sock;
    sock = nullptr;
  }

  cin.ignore();
  return 0;
}

Resource resourceRequested(const string& pathToRes) {
#ifdef DEBUG
  cout << "Info: Asking for resource: " << pathToRes << endl;
#endif

  Resource out;

  // Remove the slash in the beginning

  // If |pathToRes| is empty, then we're requesting the site root
  if (pathToRes == "/") {
#ifdef DEBUG
  cout << "\tNeed to return index.html" << endl;
#endif

    return {"index.html", "text/html", ""};
  }

  // If request had spaces in it, they would be changed into |%20|
  // TODO(Olster): User regex_replace when it's working. Make HTTPParser do it,
  // wouldn't need the path copy

  std::string pathCopy = pathToRes;

  string::size_type space = 0;
  while ((space = pathCopy.find("%20")) != string::npos) {
    // Replace 3 characters |%20| with space
    pathCopy.replace(space, 3, " ");
  }

  out.path = pathCopy;

#ifdef DEBUG
  cout << "\tResource URI: " << pathCopy << endl;
#endif

  string::size_type extensionDot = pathCopy.find_last_of('.');
  string extension = pathCopy.substr(extensionDot + 1);

#ifdef DEBUG
  cout << "\tExtension: " << extension << endl;
#endif

  // TODO(Olster): Build a map that returns MIME type instead of ifs

  // Text
  if (extension == "html" || extension == "htm") {
    out.MIMEtype = "text/html";
  }

  if (extension == "css") {
    out.MIMEtype = "text/css";
  }

  // Image
  if (extension == "ico ") {
    out.MIMEtype = "image/x-image";
  }

  if (extension == "png") {
    out.MIMEtype = "image/png";
  }

  if (extension == "gif") {
    out.MIMEtype = "image/gif";
  }

  if (extension == "jpg" || extension == "jpeg") {
    out.MIMEtype = "image/jpeg";
  }

#ifdef DEBUG
  cout << "\tReturning resource with path " << out.path << " type " << out.MIMEtype << endl;
#endif

  return out;
}

void getResource(Resource& res) {
  // Should return error code

  // TODO(Olster): This is not reliable, would fail if the
  // requested file is too big. Use chunked transfer encoding?

  FILE* file = nullptr;

  // TODO(Olster): The path to the folder containing resources will be set
  // when creating the HTTPServer as a parameter in a constructor.
  // Or make a configuration class and pass it as a parameter.

  res.path = "/home/olster/cpp_progr/Server/bin/Debug/" + res.path;

#ifdef DEBUG
  cout << "\tOpening file " << res.path << endl;
#endif

  file = fopen(res.path.c_str(), "rb");

  if (!file) {
    // TODO(Olster): Exceptions?
    res.data = R"(
    <doctype html>
    <html>
    <head>
      <title>Page not found</title>
    </head>
    <body>
      <h2>Sorry, the page you requested does not exist</h2>
    </body>
    </html>
    )";

    //res.data = "";

    res.MIMEtype = "text/html";

    return;
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
  res.data = string(buf, size - 1);

  delete [] buf;
  buf = nullptr;
}
