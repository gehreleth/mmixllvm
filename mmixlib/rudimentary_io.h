#ifndef RUDIMENTARY_IO_INCLUDED
#define RUDIMENTARY_IO_INCLUDED

#include "mmixdef.h"
#include "econtext.h"
#include "syscall_handler.h"
#include <windows.h>

namespace MmixLlvm {

class WinRudimentaryIoImpl : public SyscallHandler {
  HANDLE handleArr_[0x100];
public:
  WinRudimentaryIoImpl();
  virtual ~WinRudimentaryIoImpl();
  virtual void handleSyscall(ExecutionContext& ctx, MXUTetra opc);
private:
  void rudimentaryIo(ExecutionContext& ctx, RIO_Opc op, MXUByte z);
  MXSOcta rudimentaryIo_Fopen(ExecutionContext& ctx, const wchar_t* fn, RIO_Mode mode, MXUByte handle);
  MXSOcta rudimentaryIo_Fread(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size);
  MXSOcta rudimentaryIo_Fwrite(ExecutionContext& ctx, HANDLE h, MXUOcta buffPtr, size_t size);
  MXSOcta rudimentaryIo_Fwrite0(ExecutionContext& ctx, HANDLE h, const void* buff, size_t size);
  MXSOcta rudimentaryIo_Fputs(ExecutionContext& ctx, HANDLE h, MXUOcta string, bool isTty);
  MXSOcta rudimentaryIo_Fputws(ExecutionContext& ctx, HANDLE h, MXUOcta string, bool isTty);
  MXSOcta rudimentaryIo_FgetsTty(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size);
  MXSOcta rudimentaryIo_FgetsFd(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size);
  MXSOcta rudimentaryIo_FgetwsTty(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size);
  MXSOcta rudimentaryIo_FgetwsFd(ExecutionContext& ctx, HANDLE h, MXUOcta bufferPtr, size_t size);
  MXSOcta rudimentaryIo_FputsTty(ExecutionContext& ctx, HANDLE h, const wchar_t* str, size_t size);
};

}

#endif