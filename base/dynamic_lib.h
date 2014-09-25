#ifndef BASE_DYNAMIC_LIB_H_
#define BASE_DYNAMIC_LIB_H_

#include <string>

#include "base/build_required.h"
#include "base/path.h"

class DynamicLib {
 public:
  typedef void* DllHandle;
  explicit DynamicLib(DllHandle handle)
      : m_handle(handle) {}

  ~DynamicLib();

  // Returns pointer to exported function with name |name|.
  // Returns NULL on error.
  void* GetProc(const std::string& name);

  // Loads the dynamic library from specified path.
  // Returns NULL if library wasn't found.
  static DynamicLib* Load(const Path::StringType& path, int* err = NULL);
 private:
  DllHandle m_handle;

  DISALLOW_COPY_AND_ASSIGN(DynamicLib);
  DISALLOW_MOVE(DynamicLib);
};

#endif  // BASE_DYNAMIC_LIB_H_
