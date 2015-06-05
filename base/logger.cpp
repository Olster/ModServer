#include "base/logger.h"

#include <errno.h>

#include <cassert>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <string>

#include "base/encoding.h"
#include "base/os_info.h"

#if defined(WIN32)
#define snprintf _snprintf
#endif

Logger* Logger::m_logger = NULL;

namespace {
inline tm* GetTimeInfo() {
  time_t t;
  time(&t);

  return std::localtime(&t);
}

void LogProgramInfo() {
  // TODO(Olster): Read exe name and version from file.
  Log(INFO) << "Basic info:\n"
               "Executable: mod_server\n"
               "Version: 0.0.1\n"
               "OS: " << SystemInfo::OSName() << std::endl <<
               "OS Version: " << SystemInfo::OSVersion() << std::endl <<
               "Arch: " << SystemInfo::OSArch() << std::endl <<
               "Memory: " << SystemInfo::RAMInstalledMB() << "MB, "
               "(avail: " << SystemInfo::RAMAvailableMB() << " MB)";
}

std::string FormFileName() {
  char name[128];
  strftime(name, sizeof(name), "sthttps_%d%m%y_%H%M.log", GetTimeInfo());
  return name;
}
}  // namespace

std::ostream& Log(LogSeverity sev) {
  const char* level = "UNKNOWN";

  switch (sev) {
    case VERBOSE:
      level = "VERBOSE";
    break;

    case INFO:
      level = "INFO";
    break;

    case WARN:
      level = "WARN";
    break;

    case ERR:
      level = "ERROR";
    break;

    default:
      level = "UNKNOWN";
    break;
  }

  return Logger::GetLogger()->Stream() << std::endl << level << ": ";
}

namespace std {
std::ostream& operator<<(std::ostream& out, const wchar_t* const wideStr) {
  out << UTF16ToUTF8(wideStr);
  return out;
}
}  // namespace std

// static
bool Logger::InitLog(bool logToFile) {
  if (logToFile) {
    std::string fileName = FormFileName();
    std::ofstream* out = new std::ofstream();
    out->open(fileName);

    if (out->is_open()) {
      m_logger = new Logger(out, true);
    } else {
      delete out;

      return false;
    }
  } else {
    m_logger = new Logger(&std::cout, false);
  }

  LogProgramInfo();
  return true;
}

// static
void Logger::UninitLog() {
  delete m_logger;
  m_logger = NULL;
}

// static
inline Logger* Logger::GetLogger() {
  assert(m_logger);
  return m_logger;
}
