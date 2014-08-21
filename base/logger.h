#ifndef BASE_LOGGER_H_
#define BASE_LOGGER_H_

#include <fstream>
#include <string>

#include "base/build_required.h"

enum LogSeverity {
  VERBOSE = 0,
  INFO,
  WARN,
  ERR
};

std::ostream& Log(LogSeverity sev);

namespace std {
std::ostream& operator<<(std::ostream& out, const wchar_t* const wideStr);
inline std::ostream& operator<<(std::ostream& out, const std::wstring& wideStr) {
  return out << wideStr.c_str();
}
}  // namespace std

class Logger {
 public:
  static bool InitLog();
  static void UninitLog();

  static Logger& GetLogger();

  std::ostream& Stream() {
    return m_file;
  }
 private:
  Logger() {}
  ~Logger() {}

  // Creates unique file name based on time.
  // Move constructor applied.
  static std::string FormFileName();

  std::ofstream m_file;

  DISALLOW_COPY_AND_ASSIGN(Logger);
  DISALLOW_MOVE(Logger);
};

#endif  // BASE_LOGGER_H_
