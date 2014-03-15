#ifndef NET_IPENDPOINT_H_
#define NET_IPENDPOINT_H_

#include <cstring>
#include <cassert>

#include "base/build_required.h"

// IPv4 end point data.
class IPEndPoint {
 public:
  DISALLOW_COPY_AND_ASSIGN(IPEndPoint);
  DISALLOW_MOVE(IPEndPoint);

  IPEndPoint(const char* ip, unsigned short port)
   : m_port(port) {
    assert(ip);
    strcpy(m_ip, ip);
  }

  IPEndPoint(const std::string& ip, unsigned short port)
    : IPEndPoint(ip.c_str(), port) {}

  const char* ip() const { return m_ip; }
  unsigned short port() const { return m_port; }

  bool IsValid() const {
    // 0 port marks endpoint for dynamic port on Unix.
    return (m_ip[0] != '\0') && (m_port != 0);
  }
 private:
  char m_ip[20];
  unsigned short m_port = 0;
};

#endif // NET_IPENDPOINT_H_
