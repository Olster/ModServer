#ifndef HTTP_HEADER_H_
#define HTTP_HEADER_H_

#include <string>

class Header {
 public:
  Header(const std::string& name, const std::string value)
   : m_name(name), m_value(value) {}

  ~Header() {}

  // TODO(Olster): Check for empty name and value. Check with standart
  // if value can be empty.
  std::string ToString() const { return m_name + ": " + m_value; }
 private:
  std::string m_name;
  std::string m_value;
};

#endif  // HTTP_HEADER_H_
