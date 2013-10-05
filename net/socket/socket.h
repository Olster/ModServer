#ifndef NET_SOCKET_SOCKET_H_
#define NET_SOCKET_SOCKET_H_

#include "base/build_required.h"

namespace net {
class Socket {
 public:
  DISALLOW_COPY_AND_ASSIGN(Socket);
  DISALLOW_MOVE(Socket);
  Socket();
  ~Socket();
};
} // namespace net
#endif // NET_SOCKET_SOCKET_H_
