#ifndef BASE_BUILD_REQUIRED_H_
#define BASE_BUILD_REQUIRED_H_

// Macros to explicitly disallow copying and moving.
#define DISALLOW_COPY_AND_ASSIGN(Type)\
  Type(const Type& other);\
  void operator=(const Type& other)

#define DISALLOW_MOVE(Type)\
  Type(Type&& other);\
  Type& operator=(Type&& other)

// Mark variable as unused.
#ifndef UNUSED
#define UNUSED(var) ((void)(var))
#endif

// Safely determine the size of the array.
template <class Type, unsigned int ArrSize>
char (&arrSizeHelper(Type (&arr)[ArrSize]))[ArrSize];

#define ARR_SIZE(arr) (sizeof(arrSizeHelper(arr)))

#ifndef NULL
#define NULL 0
#endif

#endif  // BASE_BUILD_REQUIRED_H_
