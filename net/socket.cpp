#include "net/socket.h"

#include <sys/select.h>
#include <unistd.h> // for close()

namespace net {

Socket::Socket(SOCK_DOMAIN domain)
 : m_domain(domain) {

}

Socket::~Socket() {
  close(m_socket);
}

bool Socket::Close(CLOSE_TYPE type) {
  int errorCode = shutdown(m_socket, type);

  // if errorCode is 0, then the call was successful
  return errorCode == 0;
}

int Socket::Select(int maxFd, fd_set* readList,
                    fd_set* writeList, fd_set* errorList,
                    timeval* tv) {
  return select(maxFd, readList, writeList, errorList, tv);
}

void Socket::SetHandle(InternalSockType sock) {
  m_socket = sock;

  // Resetting ready flag
  m_bReady = false;
}

} // namespace net
