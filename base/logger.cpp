#include "base/logger.h"

#include <cstdarg>
#include <cassert>
#include <ctime>

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
  printf("Log file created: %s", fileName.c_str());

  logger.m_file = std::fopen(fileName.c_str(), "w");
  return logger.m_file != nullptr;
}

// static
void Logger::UninitLog() {
  FILE* file = GetLogger().m_file;
  if (file) {
    std::fclose(file);
    GetLogger().m_file = nullptr;
  }
}

// static
void Logger::Log(const char* messageFormat, ...) {
  FILE* logFile = GetLogger().m_file;
  
  assert(logFile);
  if (logFile) {
    va_list args;
    va_start(args, messageFormat);
    vfprintf(logFile, messageFormat, args);
    va_end(args);

    // TODO(Olster): This is a temp way of adding new line in the end,
    // come up with better solution.
    fprintf(logFile, "\n");
    fflush(logFile);
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
