#include "base/dynamic_lib.h"

#include <memory>

#include "gtest/gtest.h"

namespace {
#if defined(WIN32)
const wchar_t* kLibName = L"kernel32.dll";
#else
const char* kLibName = "libm.so";
#endif
}

TEST(LoadLibraryTest, LoadSystemExisting) {
  std::unique_ptr<DynamicLib> lib(DynamicLib::Load(kLibName));
  EXPECT_TRUE(lib.get() != NULL);
}

TEST(LoadLibraryTest, LoadNonExistent) {
#if defined(WIN32)
  const wchar_t* libName = L"NonExistentLibraryname.dll";
#else
  const char* libName = "NonExistentLibraryname.so";
#endif

  DynamicLib* lib = DynamicLib::Load(libName);
  ASSERT_EQ(NULL, lib);
}

TEST(LoadLibraryTest, GetExistentFunction) {
#if defined(WIN32)
  const char* function = "CloseHandle";
#else
  const char* function = "cos";
#endif

  std::unique_ptr<DynamicLib> lib(DynamicLib::Load(kLibName));
  ASSERT_TRUE(lib.get() != NULL);

  void* fnPtr = lib->GetProc(function);
  EXPECT_TRUE(fnPtr != NULL);
}

TEST(LoadLibraryTest, GetNonExistentFunction) {
  std::unique_ptr<DynamicLib> lib(DynamicLib::Load(kLibName));
  ASSERT_TRUE(lib.get() != NULL);

  void* fnPtr = lib->GetProc("DaDeeDooFunction");
  EXPECT_EQ(NULL, fnPtr);
}
