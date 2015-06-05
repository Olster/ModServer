#ifndef BASE_ENCODING_H_
#define BASE_ENCODING_H_

#include <string>

std::string UTF16ToUTF8(const wchar_t* utf16String);

inline std::string UTF16ToUTF8(const std::wstring& utf16String) {
  return UTF16ToUTF8(utf16String.c_str());
}

#endif BASE_ENCODING_H_  // BASE_ENCODING_H_
