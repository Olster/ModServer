#ifndef NET_RESOURCE_H_
#define NET_RESOURCE_H_

#include "base/object.h"

namespace net {

class Resource : public base::Object {
 public:
  Resource(const std::string& data, const std::string MIMEType);
  virtual ~Resource();

  Resource(const Resource& other) = delete;
  void operator=(const Resource& other) = delete;

  const std::string& GetData() const { return m_data; }
  const std::string& GetMIMEType() const { return m_MIMEType; }

  void SetData(const std::string& data) { m_data = data; }
  void SetMIMEType(const std::string& MIMEType) { m_MIMEType = MIMEType; }

  std::string ToString() const override { return "net::Resource."; }
 private:
  std::string m_data;
  std::string m_MIMEType;
};

} // namespace net

#endif // NET_RESOURCE_H_
