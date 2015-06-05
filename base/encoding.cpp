#include "base/encoding.h"

#include <cassert>

namespace {
// Usually strings are about 20 characters long. 1 UTF-8 char can take up to 6 bytes.
const int kStackBufSize = 20*6;
}  // namespace

std::string UTF16ToUTF8(const wchar_t* utf16String) {
  assert(utf16String);
  size_t neededLength = wcstombs(NULL, utf16String, 0);

  if (neededLength == 0 || neededLength == static_cast<size_t>(-1)) {
    return std::string();
  }

  char stackBuf[kStackBufSize] = "";

  bool freeMem = false;
  char* buf = stackBuf;

  if (neededLength >= kStackBufSize) {
    buf = new char[neededLength + 1];
    freeMem = true;
  }

  wcstombs(buf, utf16String, neededLength);
  buf[neededLength] = '\0';

  std::string out(buf, neededLength);

  if (freeMem) {
    delete [] buf;
  }

  return out;
}
