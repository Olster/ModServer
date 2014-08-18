#include "net/ip_endpoint.h"

#if defined(UNIX)
#include <arpa/inet.h>
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <cassert>
#include <cstring>

#include <string>

#if defined(WIN32)
#define snprintf _snprintf
#endif

IPEndPoint::IPEndPoint() {
  memset(m_ip, 0, sizeof(m_ip));
  m_port = 0;
}

IPEndPoint::IPEndPoint(const char* ip, unsigned short port)
    : m_port(port) {
  assert(ip);
  snprintf(m_ip, ARR_SIZE(m_ip), "%s", ip);
}

IPEndPoint::IPEndPoint(const std::string& ip, unsigned short port)
    : m_port(port) {
  assert((ip.length() > 0) && (ip.length() < ARR_SIZE(m_ip) - 1));
  snprintf(m_ip, ARR_SIZE(m_ip), "%s", ip.c_str());
}

void IPEndPoint::set_ip(const std::string& ip) {
  assert((ip.length() > 0) && (ip.length() < ARR_SIZE(m_ip) - 1));
  snprintf(m_ip, ARR_SIZE(m_ip), "%s", ip.c_str());
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
