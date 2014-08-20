#ifndef BASE_COMMAND_LINE_H_
#define BASE_COMMAND_LINE_H_

#include <map>
#include <string>
#include <vector>

#include "base/build_required.h"

class CommandLine {
 public:
#if defined(WIN32)
   typedef std::wstring StringType;
#else
   typedef std::string StringType;
#endif
   typedef StringType::value_type CharType;

   static CommandLine* ForCurrentProcess();

   // On windows argv is discarded.
   static bool Init(int argc, const char* const* argv);

   bool HasSwitch(const std::string& switchStr) const;
   StringType SwitchValue(const std::string& switchStr) const;
 private:
   CommandLine();
   ~CommandLine();

#if defined(WIN32)
   void InitFromString(const std::wstring& cmdLine);
#endif
   void InitFromArgv(int argc, const CharType* const* argv);

   typedef std::vector<StringType> StringVec;
   void InitFromStringVec(const StringVec& cmdParams);

   static CommandLine* m_currentProcCl;

   std::map<std::string, StringType> m_switches;

   DISALLOW_COPY_AND_ASSIGN(CommandLine);
   DISALLOW_MOVE(CommandLine);
};

#endif  // BASE_COMMAND_LINE_H_
