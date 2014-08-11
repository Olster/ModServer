#include "base/os_info.h"

#include <string>

#include "base/logger.h"

// static
std::string SystemInfo::OSName() {
  return "Windows NT";
}

// static
std::string SystemInfo::OSVersion() {
  return "TODO(Olster): Implement";
}

namespace {
long long QueryMemory(int /*settingName*/) {
  return 1;
}
}

// static
long long SystemInfo::RAMInstalledMB() {
  return 1;
}

// static
long long SystemInfo::RAMAvailableMB() {
  return 1;
}

// static
std::string SystemInfo::OSArch() {
  return "TODO(Olster): Implement";
}
