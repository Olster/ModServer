#ifndef BASE_LOGGER_H_
#define BASE_LOGGER_H_

#include <assert.h>

#include <string>

#include "base/build_required.h"

#include "plugin_api/plugin_log.h"

std::ostream& Log(LogSeverity sev);

namespace std {
std::ostream& operator<<(std::ostream& out, const wchar_t* const wideStr);
inline std::ostream& operator<<(std::ostream& out, const std::wstring& wideStr) {
  return out << wideStr.c_str();
}
}  // namespace std

class Logger {
 public:
  static bool InitLog(bool logToFile);
  static void UninitLog();

  static Logger* GetLogger();

  std::ostream& Stream() {
    assert(m_out);
    return *m_out;
  }
 private:
   Logger(std::ostream* out, bool deleteOut)
     : m_out(out), m_deleteOut(deleteOut) {}

  ~Logger() {
    if (m_deleteOut) {
      delete m_out;
    }
  }

  static Logger* m_logger;

  std::ostream* m_out;
  bool m_deleteOut;

  DISALLOW_COPY_AND_ASSIGN(Logger);
  DISALLOW_MOVE(Logger);
};

#endif  // BASE_LOGGER_H_
