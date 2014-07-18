#ifndef DYNAMIC_LIB_H_
#define DYNAMIC_LIB_H_

#include <string>

#include <memory>

#include "base/build_required.h"

typedef void* DllHandle;

class DynamicLib {
public:
  explicit DynamicLib(DllHandle handle)
   : m_handle(handle) {}
  
  ~DynamicLib();

  // Returns pointer to exported function with name |name|.
  // Returns NULL on error.
  void* GetProc(const std::string& name);

  // Loads the dynamic librry from specified path.
  // Returns NULL if library wasn't found.
  static DynamicLib* Load(const std::string& path);
private:
  DllHandle m_handle = NULL;

  DISALLOW_COPY_AND_ASSIGN(DynamicLib);
  DISALLOW_MOVE(DynamicLib);
};

#endif // DYNAMIC_LIB_H_
