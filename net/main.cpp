/*
  This file is intended for testing putposes only
  currently it looks like a mess where I'm trying to figure out
  the concepts of HTTP protocol
*/

#include <iostream>

#include "CServerSocket.h"

// Temps
#include <cstdio>
#include <error.h> // For perror()

using namespace std;

// TODO(Olster): Make a separate class or move to another file
struct Resource {
  string path;

  string MIMEtype;
};

Resource resRequested(const string& header);
string getRes(const string& path);

int main() {
  // Port 2563 as a default
  net::CServerSocket httpServer;
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

  net::SOCK_T acceptedSock = 0;
  net::CSocket* sock = nullptr;

  bool bAnsverAvailable = false;

  // 5 min timeout structure
  timeval tv;
  tv.tv_sec = 5 * 60;
  tv.tv_usec = 0l;

  std::string dataReceived;

  while (true) {
    timeval timeLeft = tv;

    readList = master;
    writeList = master;

    // We don't want to write to ourselves
    FD_CLR(httpServer.GetHandle(), &writeList);

    int retCode = net::CSocket::Select(maxFd + 1, &readList, &writeList, NULL, &timeLeft);

    if (retCode == 0) {
      cout << "Timeout reached" << endl;
      continue;
    } else if (retCode == -1) {
      perror("An error has occured: ");
      break;
    }

    // When Chrome is in non-incognito mode, we accept
    // 2 sockets for some reason
    // TODO(Olster): Find a cause end eliminate the problem
    if (FD_ISSET(httpServer.GetHandle(), &readList)) {
      cout << "Accepting socket" << endl;
      acceptedSock = httpServer.Accept();
      if (acceptedSock > maxFd) {
        maxFd = acceptedSock;
      }

      FD_SET(acceptedSock, &master);
    }

    if (FD_ISSET(acceptedSock, &readList)) {
      cout << "Receiving from " << acceptedSock << endl;

      if (sock) {
        delete sock;
        sock = nullptr;
      }
      sock = new net::CSocket(acceptedSock);

      if (sock->Receive(dataReceived) < 1) {
        cout << "Socket " << acceptedSock << " closed the connection" << endl;
        FD_CLR(acceptedSock, &master);
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

    if (FD_ISSET(acceptedSock, &writeList) && bAnsverAvailable) {
      cout << "Sending data to " << acceptedSock << endl;
      string headerLine = dataReceived.substr(0, dataReceived.find('\n'));

      Resource res = resRequested(headerLine);

      std::string htmlFile = "";

      if (sock) {
        string formatted = "HTTP/1.1 200 OK\nContent-Type: " + res.MIMEtype + "\nContent-Length: " + std::to_string(htmlFile.length()) + "\n\n" + htmlFile;

#ifdef DEBUG
        cout << "\n\nSending: " << formatted << endl << endl;
#endif

        int sent = sock->Send(formatted);
        cout << "Data sent: " << sent << " out of " << htmlFile.length() << "+" << endl;
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

  cin.ignore();
  return 0;
}

Resource resRequested(const string& header) {
#ifdef DEBUG
  cout << "Asking for resource with header: " << header << endl;
#endif

  Resource out {"", ""};
  // GET PATH HTTP
  string::size_type firstSpace = header.find(' ');
  string::size_type secondSpace = header.find(' ', firstSpace + 1);

  if (firstSpace == string::npos || secondSpace == string::npos) {
    // Empty resource
#ifdef DEBUG
  cout << "Couldn't get the resource URI" << header << endl;
#endif
    return Resource {"", ""};
  }

  // requested_resource.extension
  // |+2| counting that space we found and "/"
  string resourceName = header.substr(firstSpace + 2, secondSpace - firstSpace - 2);

  // If |resourceName| is empty, then we're requesting the site root
  if (resourceName.empty()) {
    return Resource{"index.html", "text/html"};
  }

  out.path = resourceName;
#ifdef DEBUG
  cout << "Resource URI: " << resourceName << " Length: " << resourceName.length() << endl;
#endif
  // Find the last dot for extension (path can have multiple dots)
  string::size_type extensionDot = resourceName.find_last_of('.');
  string extension = resourceName.substr(extensionDot + 1);

#ifdef DEBUG
  cout << "Extension: " << extension << " Length: " << extension.length() << endl;
#endif

  // TODO(Olster): Build a map that returns MIME type instead of ifs

  if (extension == "html" || extension == "htm") {
    out.MIMEtype = "text/html";
  }

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
  cout << "Returning resource with path " << out.path << " type " << out.MIMEtype << endl;
#endif

  return out;
}

string getRes(const string& path) {
  // TODO(Olster): This is not reliable, would fail if the
  // requested file is too big

  FILE* file = nullptr;
  file = fopen(path.c_str(), "rb");

  if (!file) {
    return "No such file";
  }

  char* buf = nullptr;

  string out = "";

//  while (!feof(file)) {
//    fgets(buf, 1023, file);
//    out += buf;
//  }

  fseek(file, 0, SEEK_END);
  int size = ftell(file);

  fseek(file, 0, SEEK_SET);

  buf = new char[size + 1];
  buf[size] = '\0';

  // |1| is probably 1 char/byte at a time
  // TODO(Olster): Work on this
  fread(buf, size, 1, file);

  fclose(file);
  file = nullptr;

  // NOTE: Can't do the assignment, cause binary data would be corrupt
  // the constructor that takes char as a parameter will delete all non-printed
  // characters
  out = std::string(buf, size - 1);

  delete buf;
  buf = nullptr;

  return out;
}
