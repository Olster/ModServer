#ifndef BASE_PATH_H_
#define BASE_PATH_H_

#if defined(WIN32)
#define PATH_LITERAL(x) L ## x
#else
#define PATH_LITERAL(x) x
#endif

#include <vector>
#include <string>

class Path {
 public:
#if defined(WIN32)
  typedef std::wstring StringType;
#else
  typedef std::string StringType;
#endif

  explicit Path(const StringType& path)
      : m_path(path) {}

  Path() {}
  ~Path() {}

  void set_path(const StringType& path) {
    m_path = path;
  }

  StringType Absolute() const;

  const StringType& ToString() const { return m_path; }
 private:
  StringType m_path;
};

class FileSysObject {
 public:
  explicit FileSysObject(const Path& path)
      : m_path(path) {}

  virtual ~FileSysObject() {}

  const Path& path() const { return m_path; }
 private:
  Path m_path;
};

class File : public FileSysObject {
 public:
  explicit File(const Path& path)
      : FileSysObject(path) {}
};

class Folder : public FileSysObject {
 public:
  explicit Folder(const Path& path)
      : FileSysObject(path) {}

  void GetAllSubfolders(std::vector<Folder>* subfolders,
                        bool recursive = false) const;

  void GetFilesWildcard(const Path::StringType& wildcard,
                        std::vector<File>* files) const;
};

#endif  // BASE_PATH_H_
