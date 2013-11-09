#include "resource/resource.h"

#include <cassert>
#include <map>

namespace resource {
bool Resource::Open(const std::string& path) {
  // Close previously opened file, if any.
  Close();

  // Determine MIME type by analyzing the extension.
  auto extDotPos = path.rfind('.');
  if (extDotPos != std::string::npos) {
    std::string extension = path.substr(extDotPos + 1);
    m_mimeType = MimeFromExtension(extension);
  } else {
    m_mimeType = "text/plain";
  }

  m_resFile = fopen(path.c_str(), "rb");
  return m_resFile != nullptr;
}

void Resource::Close() {
  if (m_resFile) {
    fclose(m_resFile);
    m_resFile = nullptr;
  }
}

long Resource::ResourceSizeBytes() {
  assert(m_resFile);
  if (!m_resFile) {
    return -1;
  }

  long currFilePos = ftell(m_resFile);

  fseek(m_resFile, 0, SEEK_END);
  long size = ftell(m_resFile);

  // Return to previous point in file.
  fseek(m_resFile, currFilePos, SEEK_SET);
  return size;
}

size_t Resource::Read(std::string& buffer, int bytesToRead) {
  assert(m_resFile);
  if (!m_resFile) {
    return 0;
  }

  char* temp = new char[bytesToRead];
  size_t read = fread(temp, 1, bytesToRead, m_resFile);

  buffer = std::string(temp, read);
  delete [] temp;

  return read;
}

const std::string& Resource::MimeFromExtension(const std::string& ext) {
  static std::map<const std::string, const std::string> mimes = {
    {"default", "text/plain"},
    {"htm", "text/html"}, {"html", "text/html"},
    {"css", "text/css"},
    {"png", "image/png"}, {"jpeg", "image/jpeg"}, {"jpeg", "image/jpg"},
    {"ico", "image/x-icon"},
    {"js", "application/javascript"},
    {"mp3", "audio/mpeg"}
  };

  auto it = mimes.find(ext);
  if (it == mimes.end()) {
    // It's dangerous to return a reference to temporary object.
    // Thus special entry has been added that returns "text/plain"
    // and is the first entry in the map.
    return mimes.begin()->second;
  }

  return it->second;
}
} // namespace resource
