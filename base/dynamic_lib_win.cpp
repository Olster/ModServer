#include "base/dynamic_lib.h"

#include <windows.h>

DynamicLib::~DynamicLib() {
  ::FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
  m_handle = NULL;
}

void* DynamicLib::GetProc(const std::string& name) {
  return ::GetProcAddress(reinterpret_cast<HMODULE>(m_handle), name.c_str());
}

// static
DynamicLib* DynamicLib::Load(const std::string& path) {
  DllHandle handle = NULL;
  handle = ::LoadLibraryA(path.c_str());

  if (!handle) {
    return NULL;
  }

  return new DynamicLib(handle);
}
