#include "base/dynamic_lib.h"

#include <dlfcn.h>
#include <errno.h>

DynamicLib::~DynamicLib() {
  ::dlclose(m_handle);
}

void* DynamicLib::GetProc(const std::string& name) {
  return ::dlsym(m_handle, name.c_str());
}

// static
DynamicLib* DynamicLib::Load(const std::string& path, int* err) {
  DllHandle handle = ::dlopen(path.c_str(), RTLD_LAZY);
  
  if (err) {
    *err = errno;
  }
  
  if (!handle) {
    return NULL;
  }
  
  return new DynamicLib(handle);
}

