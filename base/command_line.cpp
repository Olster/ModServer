#include "base/command_line.h"

#include <assert.h>

#if defined(WIN32)
#include <windows.h>
#pragma comment(lib, "Shell32.lib")
#endif

#include <algorithm>

#include "base/path.h"

CommandLine* CommandLine::m_currentProcCl = NULL;

namespace {
const CommandLine::CharType kSwitchPrefix = PATH_LITERAL('-');
const CommandLine::CharType kValueSeparator = PATH_LITERAL('=');
}  // namespace

// static
CommandLine* CommandLine::ForCurrentProcess() {
  assert(m_currentProcCl);
  return m_currentProcCl;
}

// static
bool CommandLine::Init(int argc, const char* const* argv) {
  if (m_currentProcCl) {
    return false;
  }

  m_currentProcCl = new CommandLine();

#if defined(WIN32)
  UNUSED(argc);
  UNUSED(argv);
  m_currentProcCl->InitFromString(::GetCommandLineW());
#else
  m_currentProcCl->InitFromArgv(argc, argv);
#endif

  return true;
}

bool CommandLine::HasSwitch(const std::string& switchStr) const {
  return m_switches.find(switchStr) != m_switches.end();
}

CommandLine::StringType CommandLine::SwitchValue(const std::string& switchStr) const {
  std::map<std::string, StringType>::const_iterator found =
    m_switches.find(switchStr);

  if (found == m_switches.end()) {
    return StringType();
  }

  return found->second;
}

CommandLine::CommandLine() {}

CommandLine::~CommandLine() {}

#if defined(WIN32)
void CommandLine::InitFromString(const std::wstring& cmdLine) {
  int argc = 0;
  wchar_t** argv = ::CommandLineToArgvW(cmdLine.c_str(), &argc);

  if (!argv) {
    return;
  }

  InitFromArgv(argc, argv);
  ::LocalFree(argv);
}
#endif

void CommandLine::InitFromArgv(int argc,
                               const CommandLine::CharType* const* argv) {
  assert(argc && argv);

  StringVec params;

  // Starting off 1 to skip the program name.
  for (int i = 1; i < argc; i++) {
    params.push_back(argv[i]);
  }

  InitFromStringVec(params);
}

namespace {
bool ParseSwitch(const CommandLine::StringType& command,
                 std::string* switchStr,
                 CommandLine::StringType* value) {
  if (command.empty()) {
    return false;
  }

  if (command[0] != kSwitchPrefix) {
    return false;
  }

  CommandLine::StringType::size_type separatorPos = command.find(kValueSeparator);
  // Starting from 1 to skip the separator.

  if (separatorPos == CommandLine::StringType::npos) {
    *switchStr = std::string(command.begin() + 1, command.end());
    value->empty();
  } else {  
    CommandLine::StringType switchNonASCII = command.substr(1, separatorPos - 1);
    *switchStr = std::string(switchNonASCII.begin(), switchNonASCII.end());
    *value = command.substr(separatorPos + 1);
  }

  return true;
}
}  // namespace

void CommandLine::InitFromStringVec(const StringVec& cmdParams) {
  m_switches.clear();

  for (StringVec::const_iterator it = cmdParams.cbegin();
       it != cmdParams.cend(); ++it) {
    std::string switchStr;
    CommandLine::StringType value;

    if (ParseSwitch(*it, &switchStr, &value)) {
      m_switches.insert(std::make_pair(switchStr, value));
    }
  }
}

