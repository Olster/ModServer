#include "base/os_info.h"

#include <fstream>
#include <sys/utsname.h>
#include <unistd.h>

#include "base/logger.h"

// static
std::string SystemInfo::OSName() {
  utsname name;
  if (uname(&name) == 0) {
    return std::string(name.sysname);
  }

  Logger::Log(Logger::WARN, "Didn't query OS name: %d", errno);
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
    Logger::Log(Logger::WARN, "Didn't query RAM %s, %d", __FUNCTION__, errno);
    return 0;
  }

  return page*pageSize;
}
}

// static
long long SystemInfo::RAMInstalledMB() {
  return QueryMemory(_SC_PHYS_PAGES)/1024/1024;
}

//static
long long SystemInfo::RAMAvailableMB() {
  return QueryMemory(_SC_AVPHYS_PAGES)/1024/1024;
}

// static
std::string SystemInfo::OSArch() {
  utsname name;
  if (uname(&name) == 0) {
    return name.machine;
  }

  Logger::Log(Logger::WARN, "Didn't get proc arch %d", errno);
  return std::string();
}
