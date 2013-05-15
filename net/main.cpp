#include <iostream>
#include "CServerSocket.h"

// Temps
#include <cstdio>
#include <error.h> // For perror()
#include <list>

#include <unistd.h> // sleep()

#include <random>

using namespace std;

string getRes(string path);
string easterEgg();

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

      if (sock) {
        delete sock;
        sock = nullptr;
      }
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
      std::string htmlFile = getRes("/home/olster/cpp_progr/Server/bin/Debug/index.html");
      //std::string htmlFile = easterEgg();
      sock->Send("HTTP/1.0 200 OK\nContent-Type: text/html\n\n" + htmlFile);

      cout << "Data sent, closing connection " << acceptedSock << endl;

      // Socket was closed, removing from master set
      if (sock) {
        delete sock;
        sock = nullptr;
      }

      FD_CLR(acceptedSock, &master);

      bAnsverAvailable = false;
    } else {
      //cout << "Not sending" << endl;
    }

    sleep(0);
  }

  cin.ignore();
  return 0;
}

string getRes(string path) {
  static FILE* file = nullptr;
  file = fopen(path.c_str(), "rb");

  if (!file) {
    return "No such file";
  }

  char buf[1024];
  buf[1023] = '\0';

  string out = "";

  while (!feof(file)) {
    fgets(buf, 1023, file);
    out += buf;
  }

  fclose(file);
  file = nullptr;

  return out;
}

string easterEgg() {
  static default_random_engine re;
	static uniform_int_distribution<int> dist(0, 7);


  static char* strings[] = {
    "<h1>Hello?</h1>",
    "<h1>I made this with my brain</h1>",
    "<h1>Can we just not?</h1>",
    "<h1>Kennan is adorable</h1>",
    "<h1>I don't speak German, but I can if you'd like</h1>",
    "<h1>When I look back on my life, it's not that I don't want to see things exactly as they happened</h1>",
    "<h1>It's just that I prefer to remember them in an artistic way.</h1>",
    "<h1>And truthfully, the lie of it all is much more honest because I invented it.</h1>"
  };

  return strings[dist(re)];
}
