/*
  This file is intended for testing putposes only
  currently it looks like a mess where I'm trying to figure out
  the concepts of HTTP protocol
*/

#include <iostream>

#include "net/ServerSocket.h"

// Temps
#include <error.h> // For perror()

using namespace std;

// TODO(Olster): Make a separate class or move to another file
struct Resource {
  string path;
  string MIMEtype;

  string data;
};

Resource resourceRequested(const string& header);
void getResource(Resource& res);

int main() {
  net::ServerSocket httpServer(2563);
  httpServer.Open();
  httpServer.Bind();
  httpServer.Listen(5);

  // Everything below is just testing.

  fd_set master;
  fd_set readList;
  fd_set writeList;

  FD_ZERO(&master);
  FD_ZERO(&readList);
  FD_ZERO(&writeList);

  FD_SET(httpServer.GetHandle(), &master);

  int maxFd = httpServer.GetHandle();

  // TODO(Olster): Do I still need |acceptedSock|?
  net::SOCK_T acceptedSock = 0;
  net::TCPSocket* sock = nullptr;

  bool bAnsverAvailable = false;

  // 5 min timeout structure
  timeval tv;
  tv.tv_sec = 5 * 60;
  tv.tv_usec = 0l;

  string dataReceived;

  while (true) {
    timeval timeLeft = tv;

    readList = master;
    writeList = master;

    // We don't want to write to ourselves
    FD_CLR(httpServer.GetHandle(), &writeList);

    int retCode = net::Socket::Select(maxFd + 1, &readList, &writeList, NULL, &timeLeft);

    if (retCode == 0) {
      cout << "Timeout reached" << endl;

      // Closing timed out connection
      // by deleting socket for now
      if (sock) {
        delete sock;
        sock = nullptr;
      }
      FD_CLR(acceptedSock, &master);

      continue;
    } else if (retCode == -1) {
      perror("An error has occured: ");
      break;
    }

    if (FD_ISSET(httpServer.GetHandle(), &readList)) {
      cout << "Accepting socket" << endl;
      sock = httpServer.Accept();

      net::SOCK_T sockFileDescr = sock->GetHandle();

      if (sockFileDescr > maxFd) {
        maxFd = sockFileDescr;
      }

      FD_SET(sockFileDescr, &master);
    }

    if (sock != nullptr && FD_ISSET(sock->GetHandle(), &readList)) {
      cout << "Receiving from " << sock->GetHandle() << endl;

      if (sock->Receive(dataReceived) < 1) {
        cout << "Socket " << sock->GetHandle() << " closed the connection" << endl;
        FD_CLR(sock->GetHandle(), &master);
        dataReceived.clear();

        delete sock;
        sock = nullptr;

        bAnsverAvailable = false;
      } else {
        cout << dataReceived << endl;
        bAnsverAvailable = true;
      }
    } else {
      //cout << "Not receiving" << endl;
    }

    if (sock != nullptr && FD_ISSET(sock->GetHandle(), &writeList) && bAnsverAvailable) {
      cout << "Sending data to " << sock->GetHandle() << endl;
      string headerLine = dataReceived.substr(0, dataReceived.find('\n'));

      Resource res = resourceRequested(headerLine);
      getResource(res);
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

        int sent = sock->Send(formatted);
        cout << "Data sent: " << sent << " out of " << res.data.length() << "+" << endl;
      } else {
        cout << "Socket was closed" << endl;
      }

      //sFD_CLR(acceptedSock, &master);

      bAnsverAvailable = false;
    } else {
      //cout << "Not sending" << endl;
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

Resource resourceRequested(const string& header) {
#ifdef DEBUG
  cout << "Asking for resource with header: " << header << endl;
#endif

  Resource out;

  // GET PATH HTTP
  string::size_type firstSpace = header.find(' ');
  string::size_type secondSpace = header.find(' ', firstSpace + 1);

  if (firstSpace == string::npos || secondSpace == string::npos) {
    // Empty resource identificator in header
#ifdef DEBUG
  cout << "\tCouldn't get the resource URI" << header << endl;
#endif
    return Resource {"", "", ""};
  }

  // requested_resource.extension
  // |+2| counting that space we found and "/"
  string resourceName = header.substr(firstSpace + 2, secondSpace - firstSpace - 2);

  // If |resourceName| is empty, then we're requesting the site root
  if (resourceName.empty()) {
    // Leaving data field empty
    // Will look more clear when we make a separate class
#ifdef DEBUG
  cout << "\tNeed to return index.html" << endl;
#endif

    return Resource {"index.html", "text/html", ""};
  }

  // If request had spaces in it, they would be changed into |%20|
  // TODO(Olster): User regex_replace when it's working
  string::size_type space = 0;
  while ((space = resourceName.find("%20")) != string::npos) {
    // Replace 3 characters |%20| with space
    resourceName.replace(space, 3, " ");
  }

  out.path = resourceName;

#ifdef DEBUG
  cout << "\tResource URI: " << resourceName << " Length: " << resourceName.length() << endl;
#endif

  string::size_type extensionDot = resourceName.find_last_of('.');
  string extension = resourceName.substr(extensionDot + 1);

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
  // Should return error code if any

  // TODO(Olster): This is not reliable, would fail if the
  // requested file is too big

  FILE* file = nullptr;

  // TODO(Olster): This should be set in cmd line at start
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
      <h2>Sorry, there is no page you requested</h2>
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

  // |1| is probably 1 char/byte at a time
  // TODO(Olster): Get detailed information
  fread(buf, size, 1, file);

  fclose(file);
  file = nullptr;

  // NOTE: Can't do the assignment, cause binary data would be corrupt
  // the constructor that takes char as a parameter will delete all non-printed
  // characters
  res.data = string(buf, size - 1);

  delete [] buf;
  buf = nullptr;
}
