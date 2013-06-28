#include "net/resource.h"

namespace net {

Resource::Resource(const std::string& data, const std::string MIMEType)
 : m_data(data), m_MIMEType(MIMEType) {
}

Resource::~Resource() {

}

Resource::Resource(Resource&& other) {
  m_data = other.m_data;
  m_MIMEType = other.m_MIMEType;
}

Resource& Resource::operator=(Resource&& other) {
  if (&other == this) {
    return *this;
  }

  m_data = other.m_data;
  m_MIMEType = other.m_MIMEType;

  return *this;
}

} // namespace net
