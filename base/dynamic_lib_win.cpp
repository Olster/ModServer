#include "base/dynamic_lib.h"

#include <windows.h>

DynamicLib::~DynamicLib() {
  ::FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
}

void* DynamicLib::GetProc(const std::string& name) {
  return ::GetProcAddress(reinterpret_cast<HMODULE>(m_handle), name.c_str());
}

// static
DynamicLib* DynamicLib::Load(const Path::StringType& path, int* err) {
  DllHandle handle = ::LoadLibraryW(path.c_str());

  if (err) {
    *err = ::GetLastError();
  }
  
  if (!handle) {
    return NULL;
  }

  return new DynamicLib(handle);
}
