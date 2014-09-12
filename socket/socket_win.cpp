#include "socket/socket.h"

// static
int Socket::InitSockets() {
  WSADATA data = {0};
  return ::WSAStartup(MAKEWORD(2, 2), &data);
}

// static
int Socket::UninitSockets() {
  return ::WSACleanup();
}
