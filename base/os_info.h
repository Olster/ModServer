#ifndef BASE_OS_INFO_H_
#define BASE_OS_INFO_H_

#include <string>
#include <vector>

#include "base/build_required.h"

class SystemInfo {
 public:
  static std::string OSName();
  static std::string OSVersion();

  static long long RAMInstalledMB();
  static long long RAMAvailableMB();

  // Returns OS architecture string.
  static std::string OSArch();
 private:
  DISALLOW_COPY_AND_ASSIGN(SystemInfo);
  DISALLOW_MOVE(SystemInfo);
};

#endif  // BASE_OS_INFO_H_
