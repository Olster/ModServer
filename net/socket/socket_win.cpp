#include "net/socket/socket.h"

#include <assert.h>

class NetworkIniter {
 public:
  NetworkIniter() {
    WSADATA data = {0};
    bool init = ::WSAStartup(MAKEWORD(2, 2), &data) == 0;
    assert(init && "Socket lib has not been inited");

    if (!init) {
      fprintf(stderr, "Windows socket lib wasn't init\n");
    }
  }

  ~NetworkIniter() {
    ::WSACleanup();
  }
};

// This object initializes and uninitializes networking library in windows.
NetworkIniter g_networkiniter;

Socket::~Socket() {
  ShutDown();
  Close();
}

bool Socket::Close(int* err) {
  int rc = closesocket(m_socket);

  if (err) {
    *err = ::WSAGetLastError();
  }

  return rc == 0;
}

bool Socket::ShutDown(ShutDownCode code, int* err) {
  int rc = shutdown(m_socket, code);

  if (err) {
    *err = ::WSAGetLastError();
  }

  return rc == 0;
}

int Socket::SocketError() {
  return ::WSAGetLastError();
}
