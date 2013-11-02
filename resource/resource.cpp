#include "resource/resource.h"

#include <cassert>

namespace resource {
bool Resource::Open(const std::string& path) {
  // Close previously opened file, if any.
  Close();

  m_resFile = fopen(path.c_str(), "rb");
  return m_resFile != nullptr;
}

void Resource::Close() {
  if (m_resFile) {
    fclose(m_resFile);
    m_resFile = nullptr;
  }
}

long Resource::FileSize() {
  long currFilePos = ftell(m_resFile);

  fseek(m_resFile, 0, SEEK_END);
  long size = ftell(m_resFile);

  // Return to previous point in file.
  fseek(m_resFile, currFilePos, SEEK_SET);
  return size;
}

size_t Resource::Read(std::string& buffer, int bytesToRead) {
  char* temp = new char[bytesToRead];
  size_t read = fread(temp, 1, bytesToRead, m_resFile);

  buffer = std::string(temp, read);
  delete [] temp;

  return read;
}
} // namespace resource
