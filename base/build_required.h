#ifndef BASE_BUILD_REQUIRED_H_
#define BASE_BUILD_REQUIRED_H_

// Macros to explicitly disallow copying and moving.
#define DISALLOW_COPY_AND_ASSIGN(Type)\
  Type(const Type& other) = delete;\
  void operator=(const Type& other) = delete
  
#define DISALLOW_MOVE(Type)\
  Type(Type&& other) = delete;\
  Type& operator=(Type&& other) = delete;
  
// Mark variable as unused.
#ifdef UNUSED
#undef UNUSED
#endif
#define UNUSED(var) ((void)(var))

// Safely determine the size of the array.
template <class Type, unsigned int ArrSize>
unsigned int (&arrSizeHelper(Type (&arr)[ArrSize]))[ArrSize];

#define ARR_SIZE(arr) (sizeof(arrSizeHelper(arr)))

#endif // BASE_BUILD_REQUIRED_H_
