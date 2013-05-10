#include <iostream>
#include "CServerSocket.h"

// Temps
#include <error.h> // For perror()
#include <unistd.h> // For close()
#include <list>

using namespace std;

// TODO(Olster): Receive and Send must be implemented in CSocket

int main() {
  // Port 2563 as a default
  CServerSocket httpServer;
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

  list<int> acceptedSocks;

  int acceptedSock = 0;
  bool bAnsverAvailable = false;

  // 3 min timeout structure
  timeval tv;
  tv.tv_sec = 3 * 60;
  tv.tv_usec = 0l;

  while (true) {
    timeval timeLeft = tv;

    readList = master;
    writeList = master;

    // We don't want to write and read ourselves
    FD_CLR(httpServer.GetHandle(), &writeList);

    int retCode = select(maxFd + 1, &readList, &writeList, NULL, &timeLeft);

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
      //acceptedSocks.push_back(acceptedSock);
    }

//    for (auto it = acceptedSocks.begin(); it != acceptedSocks.end(); ++it) {
//      if (FD_ISSET(*it, &readList)) {
//        cout << "Receiving from " << *it << endl;
//        std::string data;
//        if (httpServer.Receive(*it, data) < 1) {
//          cout << "Socket " << *it << " closed the connection" << endl;
//          FD_CLR(*it, &master);
//          acceptedSocks.erase(it);
//        }
//        cout << data << endl;
//      } else {
//        //cout << "Not receiving" << endl;
//      }
//    }

      if (FD_ISSET(acceptedSock, &readList)) {
        cout << "Receiving from " << acceptedSock << endl;
        std::string data;

        if (httpServer.Receive(acceptedSock, data) < 1) {
          cout << "Socket " << acceptedSock << " closed the connection" << endl;
          FD_CLR(acceptedSock, &master);
        }
        cout << data << endl;
        bAnsverAvailable = true;
      } else {
        //cout << "Not receiving" << endl;
      }


//    for (auto it = acceptedSocks.begin(); it != acceptedSocks.end(); ) {
//      if (FD_ISSET(*it, &writeList)) {
//        cout << "Sending data to " << *it << endl;
//        std::string htmlFile("<doctype html>\n<html><title>I am working</title><body><h1>Hey!</h1></body></html>\n");
//        httpServer.Send(*it, "HTTP/1.0 200 OK\nContent-Type: text/html\n\n" + htmlFile);
//
//        cout << "Data sent, closing connection " << *it << endl;
//
//        // Socket was closed, removing from master set
//        close(*it);
//
//        FD_CLR(*it, &master);
//        it = acceptedSocks.erase(it);
//      } else {
//        //cout << "Not sending" << endl;
//        ++it;
//      }
//    }

      if (FD_ISSET(acceptedSock, &writeList) && bAnsverAvailable) {
        cout << "Sending data to " << acceptedSock << endl;
        std::string htmlFile("<doctype html>\n<html><title>I am working</title><body><h1>Hey!</h1></body></html>\n");
        httpServer.Send(acceptedSock, "HTTP/1.0 200 OK\nContent-Type: text/html\n\n" + htmlFile);

        cout << "Data sent, closing connection " << acceptedSock << endl;

        // Socket was closed, removing from master set
        close(acceptedSock);

        FD_CLR(acceptedSock, &master);

        bAnsverAvailable = false;
      } else {
        //cout << "Not sending" << endl;
      }
  }

  cin.ignore();
  return 0;
}
