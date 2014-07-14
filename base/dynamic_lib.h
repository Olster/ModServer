#ifndef DYNAMIC_LIB_H_
#define DYNAMIC_LIB_H_

#include <string>

#include "base/build_required.h"

// TODO(Olster): Make different .cpp files for different platforms instead of #ifdef.

#ifdef WIN32
#include <windows.h>
typedef HMODULE DllHandle;
#endif

class DynamicLib {
public:
  explicit DynamicLib(DllHandle handle)
   : m_handle(handle) {}
  
  ~DynamicLib() {
#ifdef WIN32
    ::FreeLibrary(m_handle);
#endif

    m_handle = NULL;
  }

  void* GetProc(const std::string& name) {
#ifdef WIN32
    return ::GetProcAddress(m_handle, name.c_str());
#endif
  }

  static DynamicLib* Load(const std::string& path) {
    DllHandle handle = NULL;
#ifdef WIN32
    handle = ::LoadLibraryA(path.c_str());
#endif

    if (!handle) {
      return NULL;
    }

    return new DynamicLib(handle);
  }
private:
  DllHandle m_handle = NULL;

  DISALLOW_COPY_AND_ASSIGN(DynamicLib);
  DISALLOW_MOVE(DynamicLib);
};

#endif // DYNAMIC_LIB_H_
