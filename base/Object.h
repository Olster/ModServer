#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include <string>

namespace base {

class Object {
 public:
  Object() {}
  virtual ~Object() {}

  virtual std::string ToString() const = 0;
};

} // namespace base

#endif // OBJECT_H_
