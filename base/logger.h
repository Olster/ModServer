#ifndef BASE_LOGGER_H_
#define BASE_LOGGER_H_

#include <cstdio>
#include <string>

#include "base/build_required.h"

class Logger {
 public:
  static bool InitLog();
  static void UninitLog();

  static void Log(const char* messageFormat, ...);
 private:
  Logger() = default;
  ~Logger() = default;

  static Logger& GetLogger();

  // Creates unique file name based on time.
  static std::string FormFileName();

  FILE* m_file = nullptr;
};

#endif // BASE_LOGGING_H_
