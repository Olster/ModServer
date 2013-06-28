#include "net/resource.h"

namespace net {

Resource::Resource(const std::string& data, const std::string MIMEType)
 : m_data(data), m_MIMEType(MIMEType) {
}

Resource::~Resource() {

}

} // namespace net
