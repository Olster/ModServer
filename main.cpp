#include <iostream>
#include "CServerSocket.h"

// Temps
#include <error.h> // For perror()
#include <list>

using namespace std;

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
      std::string data;

      sock = new net::CSocket(acceptedSock);

      if (sock->Receive(data) < 1) {
        cout << "Socket " << acceptedSock << " closed the connection" << endl;
        FD_CLR(acceptedSock, &master);
      }
      cout << data << endl;
      bAnsverAvailable = true;
    } else {
      //cout << "Not receiving" << endl;
    }

    if (FD_ISSET(acceptedSock, &writeList) && bAnsverAvailable) {
      cout << "Sending data to " << acceptedSock << endl;
      std::string htmlFile("<doctype html>\n<html><title>I am working</title><body><h1>Hey!</h1></body></html>\n");
      sock->Send("HTTP/1.0 200 OK\nContent-Type: text/html\n\n" + htmlFile);

      cout << "Data sent, closing connection " << acceptedSock << endl;

      // Socket was closed, removing from master set
      delete sock;

      FD_CLR(acceptedSock, &master);

      bAnsverAvailable = false;
    } else {
      //cout << "Not sending" << endl;
    }
  }

  cin.ignore();
  return 0;
}
