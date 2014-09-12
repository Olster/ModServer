#ifndef NET_IP_ENDPOINT_H_
#define NET_IP_ENDPOINT_H_

#include <string>

#include "base/build_required.h"

// IPv4 end point data.
class IPEndPoint {
 public:
  IPEndPoint();
  IPEndPoint(const char* ip, unsigned short port);
  IPEndPoint(const std::string& ip, unsigned short port);

  const char* ip() const { return m_ip; }
  unsigned short port() const { return m_port; }

  void set_ip(const std::string& ip);
  void set_port(unsigned short port);

  bool IsValid() const;
 private:
  char m_ip[20];
  unsigned short m_port;

  DISALLOW_COPY_AND_ASSIGN(IPEndPoint);
  DISALLOW_MOVE(IPEndPoint);
};

#endif  // NET_IP_ENDPOINT_H_
