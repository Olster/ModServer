#include "base/logger.h"

#include <errno.h>

#include <cassert>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <string>

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
               "Memory: " << SystemInfo::RAMInstalledMB() << "MB,"
               "(avail: " << SystemInfo::RAMAvailableMB() << ')';
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

  // 7 is the length of the longest level.
  return Logger::GetLogger()->Stream() << std::endl <<
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

  if (neededLength >= kStackBufSize) {
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
