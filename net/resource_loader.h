#ifndef NET_RESOURCELOADER_H_
#define NET_RESOURCELOADER_H_

#include "base/object.h"
#include "net/http_parser.h"

namespace net {

class ResourceLoader : base::Object {
 public:
  ResourceLoader();

  std::string ToString() const override { return "net::ResourceLoader."; }
 private:
};

} // namespace net

#endif // NET_RESOURCELOADER_H_
