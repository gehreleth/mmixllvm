#include "stdafx.h"
#include "misc.h"
#include "rudimentary_io.h"

using namespace MmixLlvm;

void WinRudimentaryIoImpl::handleSyscall(ExecutionContext& ctx, MXUTetra opc)
{
  if (opc != 0) {
    if (((opc >> 16) & 0xff) == 0) {
      rudimentaryIo(ctx, static_cast<RIO_Opc>((opc >> 8) & 0xff), opc & 0xff);
    }
    ctx.resumeFromTrap();
  } else {
    ctx.setHalt(true);
  }
}

WinRudimentaryIoImpl::WinRudimentaryIoImpl()
{
  for (int i = 0; i < (sizeof(handleArr_) / sizeof(handleArr_[0])); i++) {
    handleArr_[i] = INVALID_HANDLE_VALUE;
  }
  handleArr_[0] = GetStdHandle(STD_INPUT_HANDLE);
  handleArr_[1] = GetStdHandle(STD_OUTPUT_HANDLE);
  handleArr_[2] = GetStdHandle(STD_ERROR_HANDLE);
}

WinRudimentaryIoImpl::~WinRudimentaryIoImpl() {}

void WinRudimentaryIoImpl::rudimentaryIo(ExecutionContext& ctx, RIO_Opc op, MXUByte z)
{
  switch (op) {
    case RIO_Opc::Fopen: {
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      const MXUOcta fileNamePtr = ctx.readMem<MXUOcta>(argPtr, 0);
      const RIO_Mode mode = static_cast<RIO_Mode>(ctx.readMem<MXUOcta>(argPtr, 8));
      switch (mode) {
        case RIO_Mode::TextRead:
        case RIO_Mode::TextWrite:
        case RIO_Mode::BinaryRead:
        case RIO_Mode::BinaryWrite:
        case RIO_Mode::BinaryReadWrite: {
          std::vector<char> acc;
          MXUTetra i = 0;
          MXUByte b;
          while ((b = ctx.readMem<MXUByte>(fileNamePtr, i++)) != 0) {
            acc.push_back(b);
          }
          acc.push_back('\0');
          std::wstring fileName(toWideChar(&acc[0]));
          ctx.storeSpReg(SpecialReg::rBB, rudimentaryIo_Fopen(ctx, fileName.c_str(), mode, z));
          break;
        }
        default:
          break;
      }
      break;
    }
    case RIO_Opc::Fclose:
      if (z > 2) {
        CloseHandle(handleArr_[z]);
        handleArr_[z] = INVALID_HANDLE_VALUE;
      }
      break;
    case RIO_Opc::Fread: {
      HANDLE h = handleArr_[z];
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      MXUOcta bufferPtr = ctx.readMem<MXUOcta>(argPtr, 0);
      size_t size = static_cast<size_t>(ctx.readMem<MXUOcta>(argPtr, 8));
      MXSOcta retVal = rudimentaryIo_Fread(ctx, h, bufferPtr, size);
      ctx.storeSpReg(SpecialReg::rBB, static_cast<MXSOcta>(retVal));
      break;
    }
    case RIO_Opc::Fgets: {
      HANDLE h = handleArr_[z];
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      MXUOcta bufferPtr = ctx.readMem<MXUOcta>(argPtr, 0);
      size_t size = static_cast<size_t>(ctx.readMem<MXUOcta>(argPtr, 8));
      MXSOcta retVal;
      if (GetFileType(h) == FILE_TYPE_CHAR) {
        retVal = rudimentaryIo_FgetsTty(ctx, h, bufferPtr, size);
      } else {
        retVal = rudimentaryIo_FgetsFd(ctx, h, bufferPtr, size);
      }
      ctx.storeSpReg(SpecialReg::rBB, static_cast<MXSOcta>(retVal));
      break;
    }
    case RIO_Opc::Fgetws: {
      HANDLE h = handleArr_[z];
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      MXUOcta bufferPtr = ctx.readMem<MXUOcta>(argPtr, 0);
      size_t size = static_cast<size_t>(ctx.readMem<MXUOcta>(argPtr, 8));
      MXSOcta retVal;
      if (GetFileType(h) == FILE_TYPE_CHAR) {
        retVal = rudimentaryIo_FgetwsTty(ctx, h, bufferPtr, size);
      } else {
        retVal = rudimentaryIo_FgetwsFd(ctx, h, bufferPtr, size);
      }
      ctx.storeSpReg(SpecialReg::rBB, static_cast<MXSOcta>(retVal));
      break;
    }
    case RIO_Opc::Fwrite: {
      HANDLE h = handleArr_[z];
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      MXUOcta buffPtr = ctx.readMem<MXUOcta>(argPtr, 0);
      size_t size = static_cast<size_t>(ctx.readMem<MXUOcta>(argPtr, 8));
      MXSOcta retVal = rudimentaryIo_Fwrite(ctx, h, buffPtr, size);
      ctx.storeSpReg(SpecialReg::rBB, static_cast<MXSOcta>(retVal));
      break;
    }
    case RIO_Opc::Fputs: {
      HANDLE h = handleArr_[z];
      const MXUOcta string = ctx.readSpReg(SpecialReg::rBB);
      ctx.storeSpReg(SpecialReg::rBB, rudimentaryIo_Fputs(ctx, h, string, GetFileType(h) == FILE_TYPE_CHAR));
      break;
    }
    case RIO_Opc::Fputws: {
      HANDLE h = handleArr_[z];
      const MXUOcta string = ctx.readSpReg(SpecialReg::rBB);
      ctx.storeSpReg(SpecialReg::rBB, rudimentaryIo_Fputws(ctx, h, string, GetFileType(h) == FILE_TYPE_CHAR));
      break;
    }
    case RIO_Opc::Fseek: {
      HANDLE h = handleArr_[z];
      const MXUOcta argPtr = ctx.readSpReg(SpecialReg::rBB);
      MXSOcta offset = static_cast<MXSOcta>(ctx.readMem<MXUOcta>(argPtr, 0));
      DWORD moveMethod;
      if (offset >= 0) {
        moveMethod = FILE_BEGIN;
      } else {
        moveMethod = FILE_END;
        offset += 1;
      }
      LARGE_INTEGER li = { 0 };
      li.QuadPart = offset;
      if (SetFilePointer(h, li.LowPart, &li.HighPart, moveMethod)) {
        ctx.storeSpReg(SpecialReg::rBB, 0);
      } else {
        ctx.storeSpReg(SpecialReg::rBB, -1);
      }
      break;
    }
    case RIO_Opc::Ftell: {
      HANDLE h = handleArr_[z];
      LARGE_INTEGER liOfs = { 0 };
      LARGE_INTEGER liNew = { 0 };
      if (SetFilePointerEx(h, liOfs, &liNew, FILE_CURRENT)) {
        ctx.storeSpReg(SpecialReg::rBB, liNew.QuadPart);
      } else {
        ctx.storeSpReg(SpecialReg::rBB, -1);
      }
      break;
    }
    default:
      break;
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fopen(ExecutionContext& ctx, const wchar_t* fn, RIO_Mode mode, MXUByte z)
{
  if (z < 3) {
    return -1;
  }
  DWORD creationDisposition = 0;
  DWORD desiredAccess = 0;
  switch (mode) {
    case RIO_Mode::TextRead:
    case RIO_Mode::BinaryRead:
      desiredAccess = GENERIC_READ;
      creationDisposition = OPEN_EXISTING;
      break;
    case RIO_Mode::TextWrite:
    case RIO_Mode::BinaryWrite:
      desiredAccess = GENERIC_WRITE;
      creationDisposition = CREATE_ALWAYS;
      break;
    case RIO_Mode::BinaryReadWrite:
      desiredAccess = GENERIC_READ | GENERIC_WRITE;
      creationDisposition = OPEN_ALWAYS;
  }
  HANDLE h = CreateFileW(fn, desiredAccess, FILE_SHARE_READ, NULL, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
  if (h != INVALID_HANDLE_VALUE) {
    handleArr_[z] = h;
    return 0;
  } else {
    return -1;
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fread(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size)
{
  MXUByte* tmp = static_cast<MXUByte*>(_alloca(size));
  DWORD bytesRead;
  if (ReadFile(h, tmp, static_cast<DWORD>(size), &bytesRead, NULL) != 0) {
    ctx.importExternalBuffer(tmp, bufferPtr, bytesRead);
    return static_cast<MXSOcta>(bytesRead) - size;
  } else {
    return static_cast<MXSOcta>(-1) - size;
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_FgetsTty(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size)
{
  wchar_t* tmp = static_cast<wchar_t*>(_alloca(size * sizeof(wchar_t)));
  DWORD charsRead;
  ReadConsoleW(h, tmp, static_cast<DWORD>(size), &charsRead, NULL);
  if (charsRead >= 2) {
    if (tmp[charsRead - 1] == '\n') {
      tmp[charsRead - 1] = '\0';
      --charsRead;
    }
    if (tmp[charsRead - 1] == '\r') {
      tmp[charsRead - 1] = '\0';
      --charsRead;
    }
  }
  std::string mbs = toMultibyte(tmp);
  ctx.importExternalBuffer(mbs.c_str(), bufferPtr, mbs.length() + 1);
  return mbs.length();
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_FgetsFd(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size)
{
  std::vector<char> buff;
  char c[1];
  DWORD bytesRead;
  do {
    if (!ReadFile(h, c, sizeof(c), &bytesRead, NULL))
      bytesRead = 0;
    if (bytesRead)
      buff.push_back(c[0]);
  } while (buff.size() <= size && bytesRead != 0 && c[0] != '\n');
  if (buff[buff.size() - 1] == '\n') {
    buff.pop_back(); // Strip LF
  }
  if (buff[buff.size() - 1] == '\r') {
    buff.pop_back(); // Strip CR
  }
  buff.push_back('\0');
  ctx.importExternalBuffer(&buff[0], bufferPtr, buff.size() + 1);
  return buff.size();
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_FgetwsTty(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size)
{
  wchar_t* tmp = static_cast<wchar_t*>(_alloca(size * sizeof(wchar_t)));
  DWORD charsRead;
  if (ReadConsoleW(h, tmp, static_cast<DWORD>(size), &charsRead, NULL)) {
    if (charsRead >= 2) {
      if (tmp[charsRead - 1] == '\n') {
        tmp[charsRead - 1] = '\0';
        --charsRead;
      }
      if (tmp[charsRead - 1] == '\r') {
        tmp[charsRead - 1] = '\0';
        --charsRead;
      }
    }
    for (size_t i = 0; i < charsRead; i++) {
      ctx.storeMem<MXUWyde>(0, bufferPtr, tmp[i]);
      bufferPtr += 2;
    }
    ctx.storeMem<MXUWyde>(0, bufferPtr, L'\0');
    return charsRead + 1;
  } else {
    return -1;
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_FgetwsFd(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size)
{
  std::vector<char> buff;
  char c[1];
  DWORD bytesRead;
  do {
    if (!ReadFile(h, c, sizeof(c), &bytesRead, NULL))
      bytesRead = 0;
    if (bytesRead)
      buff.push_back(c[0]);
  } while (buff.size() <= size && bytesRead != 0 && c[0] != '\n');
  if (buff[buff.size() - 1] == '\n') {
    buff.pop_back(); // Strip LF
  }
  if (buff[buff.size() - 1] == '\r') {
    buff.pop_back(); // Strip CR
  }
  buff.push_back('\0');
  std::wstring ws = toWideChar(&buff[0]);
  for (auto q = ws.begin(); q != ws.end(); q++) {
    ctx.storeMem<MXUWyde>(0, bufferPtr, *q);
    bufferPtr += 2;
  }
  ctx.storeMem<MXUWyde>(0, bufferPtr, L'\0');
  return ws.size();
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fwrite(ExecutionContext& ctx, HANDLE h, MXUOcta buffPtr, size_t size)
{
  MXUByte* tmp = static_cast<MXUByte*>(_alloca(size));
  ctx.exportExternalBuffer(buffPtr, tmp, size);
  return rudimentaryIo_Fwrite0(ctx, h, tmp, size);
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fwrite0(ExecutionContext& ctx, HANDLE h, const void* buff, size_t size)
{
  DWORD bytesWritten;
  if (WriteFile(h, buff, static_cast<DWORD>(size), &bytesWritten, NULL)) {
    return static_cast<MXSOcta>(bytesWritten) - size;
  } else {
    return -static_cast<MXSOcta>(size);
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fputs(ExecutionContext& ctx, HANDLE h, MXUOcta string, bool isTty)
{
  std::vector<MXUByte> tmp;
  size_t i = 0;
  MXUByte b;
  while ((b = ctx.readMem<MXUByte>(string, i++)) != 0) {
    tmp.push_back(static_cast<char>(b));
  }
  tmp.push_back('\0');
  if (isTty) {
    std::wstring wideStr = toWideChar((const char *)&tmp[0]);
    return rudimentaryIo_FputsTty(ctx, h, wideStr.c_str(), wideStr.size());
  } else {
    return std::max(rudimentaryIo_Fwrite0(ctx, h, &tmp[0], tmp.size() - 1), static_cast<MXSOcta>(-1));
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_Fputws(ExecutionContext& ctx, HANDLE h, MXUOcta string, bool isTty)
{
  std::vector<wchar_t> tmp;
  size_t i = 0;
  MXUWyde w;
  while ((w = ctx.readMem<MXUWyde>(string, i)) != 0) {
    tmp.push_back(static_cast<wchar_t>(w));
    i += 2;
  }
  tmp.push_back('\0');
  if (isTty) {
    return rudimentaryIo_FputsTty(ctx, h, &tmp[0], tmp.size());
  } else {
    std::string mbstr = toMultibyte(&tmp[0]);
    return std::max(rudimentaryIo_Fwrite0(ctx, h, mbstr.c_str(), mbstr.length()), static_cast<MXSOcta>(-1));
  }
}

MXSOcta WinRudimentaryIoImpl::rudimentaryIo_FputsTty(ExecutionContext& ctx, HANDLE h, const wchar_t* str, size_t size)
{
  DWORD bytesWritten;
  if (WriteConsoleW(h, str, static_cast<DWORD>(size), &bytesWritten, NULL)) {
    return static_cast<MXSOcta>(bytesWritten);
  } else {
    return static_cast<MXSOcta>(-1);
  }
}