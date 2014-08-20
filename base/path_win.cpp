#include "base/path.h"

#include <windows.h>

Path::StringType Path::Absolute() const {
  StringType::value_type buf[MAX_PATH*2] = PATH_LITERAL("");

  // TODO(Olster): Check for return value and errors.
  DWORD res = ::GetFullPathNameW(m_path.c_str(), MAX_PATH*2, buf, NULL);
  if (res > 0) {
    return StringType(buf);
  }

  return StringType();
}

void Folder::GetAllSubfolders(std::vector<Folder>* subfolders,
                              bool recursive, int* err) const {
  WIN32_FIND_DATAW findData = {0};
  HANDLE fileFound = ::FindFirstFileW((path().ToString() +
                                       PATH_LITERAL("/*")).c_str(), &findData);

  if (fileFound == INVALID_HANDLE_VALUE) {
    if (err) {
      *err = ::GetLastError();
    }

    return;
  }

  do {
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if (wcscmp(findData.cFileName, L"..") == 0 ||
          wcscmp(findData.cFileName, L".") == 0) {
        continue;
      }
      
      Folder subfolder(Path(path().ToString() + PATH_LITERAL('/') +
        findData.cFileName));
      subfolders->push_back(subfolder);

      if (recursive) {
        subfolder.GetAllSubfolders(subfolders, recursive);
      }
    }
  } while (::FindNextFileW(fileFound, &findData));

  ::FindClose(fileFound);
}

void Folder::GetFilesWildcard(const Path::StringType& wildcard,
                              std::vector<File>* files,
                              int* err) const {
  files->clear();

  WIN32_FIND_DATAW findData = {0};
  HANDLE fileFound = ::FindFirstFileW((path().ToString() + PATH_LITERAL('/') +
                                       wildcard).c_str(), &findData);

  if (fileFound == INVALID_HANDLE_VALUE) {
    if (err) {
      *err = ::GetLastError();
    }

    return;
  }

  do {
    if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
      files->push_back(File(Path(path().ToString() + PATH_LITERAL('/') +
        findData.cFileName)));
    }
  } while (::FindNextFileW(fileFound, &findData));

  ::FindClose(fileFound);
}
