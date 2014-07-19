#include "base/logger.h"

#include <cassert>
#include <cstdarg>
#include <ctime>
#include <errno.h>

namespace {
inline tm* GetTimeInfo() {
  time_t t;
  time(&t);

  return std::localtime(&t);
}
}

// static
bool Logger::InitLog() {
  // If file is open, we've already initialized logging.
  Logger& logger = GetLogger();
  if (logger.m_file) {
    return true;
  }

  std::string fileName = FormFileName();
  logger.m_file = std::fopen(fileName.c_str(), "w");

  if (logger.m_file != nullptr) {
      printf("Log file created: %s\n", fileName.c_str());
      return true;
  } else {
      fprintf(stderr, "Log file '%s' wasn't created: %d\n",
              fileName.c_str(), errno);
  }

  return false;
}

// static
void Logger::UninitLog() {
  FILE* file = GetLogger().m_file;
  if (file) {
    std::fclose(file);
    GetLogger().m_file = nullptr;
  }
}

namespace {
const char* SeverityString(Logger::Severity sev) {
  const char* out = "UNKNOWN";

  switch (sev) {
    case Logger::VERBOSE:
      out = "VERBOSE";
    break;

    case Logger::INFO:
      out = "INFO";
    break;

    case Logger::WARN:
      out = "WARNING";
    break;

    case Logger::ERR:
      out = "ERROR";
    break;

    default:
    break;
  }

  return out;
}
}

// static
void Logger::Log(Severity sev, const char* messageFormat, ...) {
  FILE* logFile = GetLogger().m_file;
  
  assert(logFile);
  if (logFile) {
    char message[1024];

    int bytesUsed = sprintf(message, "%s: ", SeverityString(sev));

    va_list args;
    va_start(args, messageFormat);

    bytesUsed = vsprintf(message + bytesUsed, messageFormat, args);

    // Writing outside of array might have crashed the program before this assert.
    assert(bytesUsed < sizeof(message));

    const char* const outFormat = "%s\n";
    fprintf(logFile, outFormat, message);
    printf(outFormat, message);
  }
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
