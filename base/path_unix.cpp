#include "base/path.h"

#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

Path::StringType Path::Absolute() const {
  StringType::value_type buf[MAX_PATH*2] = PATH_LITERAL("");

  StringType::value_type* pathStart = realpath(m_path.c_str(),
                                               buf);

  if (!pathStart) {
    return StringType();
  }

  return StringType(pathStart);
}

void Folder::GetAllSubfolders(std::vector<Folder>* subfolders,
                              bool recursive, int* err) const {
  DIR* dir = opendir(path().ToString().c_str());

  if (!dir) {
    if (err) {
      *err = errno;
    }

    return;
  }

  dirent dentBuf = {0};
  dirent* dent = NULL;

  while (readdir_r(dir, &dentBuf, &dent) == 0 && dent) {
    if ((strcmp("..", dentBuf.d_name) == 0) ||
        (strcmp(".", dentBuf.d_name)) == 0) {
      continue;
    }

    struct stat st;

    if (fstatat(dirfd(dir), dentBuf.d_name, &st, 0) == 0) {
      if (S_ISDIR(st.st_mode)) {
        Folder subfolder(Path(path().ToString() + PATH_LITERAL('/') +
                              dentBuf.d_name));

        subfolders->push_back(subfolder);

        if (recursive) {
          subfolder.GetAllSubfolders(subfolders, recursive);
        }
      }
    } else {
      if (err) {
        *err = errno;
      }

      continue;
    }
  }

  closedir(dir);
}

void Folder::GetFilesWildcard(const Path::StringType& wildcard,
                              std::vector<File>* files,
                              int* err) const {
  files->clear();

  DIR* dir = opendir(path().ToString().c_str());

  if (!dir) {
    if (err) {
      *err = errno;
    }

    return;
  }

  dirent dentBuf = {0};
  dirent* dent = NULL;

  while (readdir_r(dir, &dentBuf, &dent) == 0 && dent) {
    if ((strcmp("..", dentBuf.d_name) == 0) ||
        (strcmp(".", dentBuf.d_name)) == 0) {
      continue;
    }

    struct stat st;
    if (fstatat(dirfd(dir), dentBuf.d_name, &st, 0) == 0) {
      if (!S_ISDIR(st.st_mode)) {
        bool patternMatch = fnmatch(wildcard.c_str(), dentBuf.d_name, 0) == 0;
        if (patternMatch) {
          files->push_back(File(Path(path().ToString() + PATH_LITERAL('/') +
                                     dentBuf.d_name)));
        }
      }
    } else {
      if (err) {
        *err = errno;
      }

      continue;
    }
  }

  closedir(dir);
}
