#ifndef BASE_BUILD_REQUIRED_H_
#define BASE_BUILD_REQUIRED_H_

#define DISALLOW_COPY_AND_ASSIGN(Type)\
  Type(const Type&) = delete;\
  void operator=(const Type&) = delete

#define DISALLOW_MOVE(Type)\
  Type(Type&&) = delete;\
  void operator=(Type&&) = delete


#endif // BASE_BUILD_REQUIRED_H_
