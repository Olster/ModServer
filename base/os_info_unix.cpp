#include "base/os_info.h"

#include <sys/utsname.h>
#include <unistd.h>

#include <fstream> // NOLINT
#include <string>

#include "base/logger.h"

// static
std::string SystemInfo::OSName() {
  utsname name;
  if (uname(&name) == 0) {
    return std::string(name.sysname);
  }

  Log(WARN) << "Didn't query OS name: " << errno;
  return std::string();
}

// static
std::string SystemInfo::OSVersion() {
  std::ifstream versionFile;
  versionFile.open("/proc/sys/kernel/version");

  // No need to check if the file was opened, getline will return false if the
  // file wasn't opened.
  std::string version;
  if (!std::getline(versionFile, version)) {
    return std::string();
  }

  return version;
}

namespace {
long long QueryMemory(int settingName) {
  long long page = sysconf(settingName);
  long long pageSize = sysconf(_SC_PAGESIZE);

  if ((page < 0) || (pageSize < 0)) {
    Log(WARN) << "Didn't query RAM " << errno;
    return 0;
  }

  return page*pageSize;
}
}  // namespace

// static
long long SystemInfo::RAMInstalledMB() {
  return QueryMemory(_SC_PHYS_PAGES)/1024/1024;
}

// static
long long SystemInfo::RAMAvailableMB() {
  return QueryMemory(_SC_AVPHYS_PAGES)/1024/1024;
}

// static
std::string SystemInfo::OSArch() {
  utsname name;
  if (uname(&name) == 0) {
    return name.machine;
  }

  Log(WARN) << "Didn't get proc arch " << errno;
  return std::string();
}
