#include "base/logger.h"

#include <errno.h>

#include <cassert>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include <iomanip>
#include <string>

#include "base/os_info.h"

#if defined(WIN32)
#define snprintf _snprintf
#endif

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
               "Memory: " << SystemInfo::RAMInstalledMB() << "MB,"
               "(avail: " << SystemInfo::RAMAvailableMB() << ')';
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

  // 7 is the length of the longest level.
  return Logger::GetLogger().Stream() << std::endl <<
    std::setw(7) << level << ": ";
}

namespace std {
const int kStackBufSize = 2048;

std::ostream& operator<<(std::ostream& out, const wchar_t* const wideStr) {
  assert(wideStr);
  size_t neededLength = wcstombs(NULL, wideStr, 0);

  if (neededLength == 0 || neededLength == static_cast<size_t>(-1)) {
    return out;
  }

  char stackBuf[kStackBufSize];

  bool freeMem = false;
  char* buf = stackBuf;

  if (neededLength >= stackBufSize) {
    buf = new char[neededLength + 1];
    freeMem = true;
  }

  wcstombs(buf, wideStr, neededLength);
  buf[neededLength] = '\0';

  out << buf;

  if (freeMem) {
    delete[] buf;
  }

  return out;
}
}  // namespace std

// static
bool Logger::InitLog() {
  // If file is open, we've already initialized logging.
  Logger& logger = GetLogger();
  if (logger.m_file.is_open()) {
    return true;
  }

  std::string fileName = FormFileName();
  logger.m_file.open(fileName);

  if (logger.m_file.is_open()) {
    logger.m_file << "Log file created " << fileName;

    LogProgramInfo();
    return true;
  } else {
    fprintf(stderr, "Log file '%s' wasn't created: %d\n",
            fileName.c_str(), errno);
  }

  return false;
}

// static
void Logger::UninitLog() {
  GetLogger().m_file.close();
}

// static
inline Logger& Logger::GetLogger() {
  static Logger logger;
  return logger;
}

// static
std::string Logger::FormFileName() {
  char name[128];
  strftime(name, sizeof(name), "sthttps_%d%m%y_%H%M.log", GetTimeInfo());
  return name;
}
