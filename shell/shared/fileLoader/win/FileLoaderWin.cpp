/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <shell/shared/fileLoader/win/FileLoaderWin.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <igl/Common.h>
#include <iterator>
#include <string>

#if IGL_PLATFORM_WIN
#include <windows.h>
#endif

namespace {

std::string findSubdir(const char* subdir, const std::string& fileName) {
  std::filesystem::path dir = std::filesystem::current_path();
  // find `subdir` somewhere above our current directory
  while (dir != std::filesystem::current_path().root_path() &&
         !std::filesystem::exists(dir / subdir)) {
    dir = dir.parent_path();
  }

  std::filesystem::path fullPath = (dir / subdir / fileName);
  if (std::filesystem::exists(fullPath)) {
    return fullPath.string();
  }

  dir = std::filesystem::current_path();

  fullPath = (dir / "images/" / fileName);
  if (std::filesystem::exists(fullPath)) {
    return fullPath.string();
  }

  return std::string();
}

} // namespace

namespace igl::shell {

FileLoaderWin::FileLoaderWin() {
#if IGL_PLATFORM_WIN
  wchar_t path[MAX_PATH] = {0};
  if (IGL_VERIFY(GetModuleFileNameW(NULL, path, MAX_PATH) != 0)) {
    basePath_ = std::filesystem::path(path).parent_path().string();
  }
#endif
}

FileLoader::FileData FileLoaderWin::loadBinaryData(const std::string& fileName) {
  return loadBinaryDataInternal(fullPath(fileName));
}

bool FileLoaderWin::fileExists(const std::string& fileName) const {
  std::ifstream file(fileName, std::ios::binary);
  auto exists = (file.rdstate() & std::ifstream::failbit) == 0;
  file.close();
  return exists;
}

std::string FileLoaderWin::basePath() const {
  // Return executable path:
  return basePath_;
}

std::string FileLoaderWin::fullPath(const std::string& fileName) const {
  if (std::filesystem::exists(fileName)) {
    return fileName;
  }

  auto fullPath = std::filesystem::path(basePath_) / fileName;
  if (std::filesystem::exists(fullPath)) {
    return fullPath.string();
  }

  fullPath = std::filesystem::path("shell/resources/images") / fileName;
  if (std::filesystem::exists(fullPath)) {
    return fullPath.string();
  }

  constexpr std::array<const char*, 5> folders = {"shell/resources/images/",
                                                  "samples/resources/images/",
                                                  "samples/resources/models/",
                                                  "samples/resources/fonts/",
                                                  "samples/resources/fonts/optimistic"};

  // find folders somewhere above our current directory
  for (const char* folder : folders) {
    if (std::string p = findSubdir(folder, fileName); !p.empty()) {
      return p;
    }
  }

  IGL_ASSERT_NOT_REACHED();
  return "";
}

} // namespace igl::shell
