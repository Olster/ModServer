#ifndef BASE_LOGGER_H_
#define BASE_LOGGER_H_

#include <string>

#include "base/build_required.h"

class Logger {
 public:
  static bool InitLog();
  static void UninitLog();

  enum Severity {
    VERBOSE = 0,
    INFO,
    WARN,
    ERR
  };

  static void Log(Severity sev, const char* messageFormat, ...);
 private:
  Logger()
   : m_file(NULL) {}
  ~Logger() {}

  static Logger& GetLogger();

  // Creates unique file name based on time.
  // Move constructor applied.
  static std::string FormFileName();

  FILE* m_file;
};

#endif // BASE_LOGGING_H_
