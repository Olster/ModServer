#include "net/ip_endpoint.h"

#include <assert.h>

#if defined(UNIX)
// TODO(Olster): Figure out the correct header for UNIX.
#else
#include <winsock2.h>
#endif

IPEndPoint::IPEndPoint(const char* ip, unsigned short port)
 : m_port(port) {
  assert(ip);
  strcpy(m_ip, ip);
}

IPEndPoint::IPEndPoint(const std::string& ip, unsigned short port)
 : IPEndPoint(ip.c_str(), port) {}

bool IPEndPoint::IsValid() const {
  if (inet_addr(m_ip) == INADDR_NONE) {
    return false;
  }

  if (m_port == 0) {
    return false;
  }

  return true;
}
