#ifndef RESOURCE_RESOURCE_H_
#define RESOURCE_RESOURCE_H_

#include "base/build_required.h"

#include <cstdio>
#include <string>

namespace resource {
class Resource {
 public:
  DISALLOW_COPY_AND_ASSIGN(Resource);
  DISALLOW_MOVE(Resource);

  Resource() = default;
  ~Resource() {
    Close();
  }

  // Returns true if file was opened successfully.
  bool Open(const std::string& path);

  // Closes resource file.
  void Close();

  // Returns the size of the resource.
  long ResourceSizeBytes();

  // Reads |bytesToRead| bytes into |buffer| from file.
  // Returns number of bytes read.
  size_t Read(std::string& buffer, int bytesToRead);

  // Returns the MIME type of current resource.
  const std::string& MimeType() const { return m_mimeType; }
 private:
  const std::string& MimeFromExtension(const std::string& ext);

  std::string m_mimeType;
  FILE* m_resFile = nullptr;
};
}

#endif // RESOURCE_RESOURCE_H_
