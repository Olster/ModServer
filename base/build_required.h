#ifndef BASE_BUILD_REQUIRED_H_
#define BASE_BUILD_REQUIRED_H_

<<<<<<< HEAD
// Macros to explicitly disallow copying and moving.
=======
// Marcos to explicitly disallow copying and moving.
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d
#define DISALLOW_COPY_AND_ASSIGN(Type)\
  Type(const Type& other) = delete;\
  void operator=(const Type& other) = delete
  
#define DISALLOW_MOVE(Type)\
  Type(Type&& other) = delete;\
  Type& operator=(Type&& other) = delete;
  
// Mark variable as unused.
#ifdef UNUSED
#undef UNUSED
<<<<<<< HEAD
#endif

#define UNUSED(var) ((void)(var))

// Safely determine the size of the array.
template <class Type, unsigned int ArrSize>
unsigned int (&arrSizeHelper(Type (&arr)[ArrSize]))[ArrSize];
=======
#define UNUSED(var) ((void)(var))
#endif

// Safely determine the size of the array.
template <class Type, size_t ArrSize>
size_t (&arrSizeHelper(T (&arr[ArrSize])))[ArrSize];
>>>>>>> cb689a30a12ee8217ce7ae14185c13bc0f44396d

#define ARR_SIZE(arr) (sizeof(arrSizeHelper(arr)))

#endif // BASE_BUILD_REQUIRED_H_
