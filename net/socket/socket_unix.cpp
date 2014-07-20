#include "net/socket/socket.h"

#include <errno.h>
#include <unistd.h>

Socket::~Socket() {
  ShutDown();
  Close();
}

bool Socket::Close(int* err) {
  int rc = close(m_socket);
  if (err) {
    *err = errno;
  }

  return rc > -1;
}

bool Socket::ShutDown(ShutDownCode code, int* err) {
  int rc = shutdown(m_socket, code);
  if (err) {
    *err = errno;
  }

  return rc != 0;
}

int Socket::SocketError() {
  return errno;
}
