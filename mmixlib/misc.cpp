#include "stdafx.h"
#include "misc.h"
#include <windows.h>

using namespace MmixLlvm;

namespace {
std::atomic<uint64_t> seed = 0l;
}

std::string MmixLlvm::genUniq(const char* prefix)
{
  char buff[21] = { 0 };
  _ui64toa_s(++seed, buff, sizeof(buff), 10);
  return std::string(prefix) += buff;
}

std::string MmixLlvm::toMultibyte(const wchar_t* src) {
  int length = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL) + 1;
  char* buff = static_cast<char *>(_alloca(length));
  WideCharToMultiByte(CP_ACP, 0, src, -1, buff, length, NULL, NULL);
  return std::string(buff);
}

std::wstring MmixLlvm::toWideChar(const char* src) {
  int length = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0) + 1;
  wchar_t* buff = static_cast<wchar_t *>(_alloca(length * sizeof(wchar_t)));
  MultiByteToWideChar(CP_ACP, 0, src, -1, buff, length);
  return std::wstring(buff);
}