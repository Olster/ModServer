#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <string>

class CommandLine {
 public:
   static bool Init(int argc, const char** const argv);

   // Returns command line for current process.
   static CommandLine& ProcessCommandLine();

   bool HasSwitch(const std::string& switchString);
 private:
   CommandLine() = delete;
};

#endif // COMMAND_LINE_H_
