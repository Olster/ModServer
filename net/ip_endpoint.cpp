#include "net/ip_endpoint.h"

#include <assert.h>

#if defined(UNIX)
// TODO(Olster): Figure out the correct header for UNIX.
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

IPEndPoint::IPEndPoint(const char* ip, unsigned short port)
 : m_port(port) {
  assert(ip);
  strcpy(m_ip, ip);
}

IPEndPoint::IPEndPoint(const std::string& ip, unsigned short port)
 : m_port(port) {
  assert((ip.length() > 0) && (ip.length() < ARR_SIZE(m_ip) - 1));
  strcpy(m_ip, ip.c_str());
}

void IPEndPoint::set_ip(const std::string& ip) {
  assert((ip.length() > 0) && (ip.length() < ARR_SIZE(m_ip) - 1));
  strcpy(m_ip, ip.c_str());
}

void IPEndPoint::set_port(unsigned short port) {
  m_port = port;
}

bool IPEndPoint::IsValid() const {
  if (inet_addr(m_ip) == INADDR_NONE) {
    return false;
  }

  if (m_port == 0) {
    return false;
  }

  return true;
}
