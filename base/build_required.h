#ifndef BASE_BUILD_REQUIRED_H_
#define BASE_BUILD_REQUIRED_H_

// Marcos to explicitly disallow copying and moving.
#define DISALLOW_COPY_AND_ASSIGN(Type)\
  Type(const Type& other) = delete;\
  void operator=(const Type& other) = delete
  
#define DISALLOW_MOVE(Type)\
  Type(Type&& other) = delete;\
  Type& operator=(Type&& other) = delete;
  
// Mark variable as unused.
#ifdef UNUSED
#undef UNUSED
#define UNUSED(var) ((void)(var))
#endif

// Safely determine the size of the array.
template <class Type, size_t ArrSize>
size_t (&arrSizeHelper(T (&arr[ArrSize])))[ArrSize];

#define ARR_SIZE(arr) (sizeof(arrSizeHelper(arr)))

#endif // BASE_BUILD_REQUIRED_H_
