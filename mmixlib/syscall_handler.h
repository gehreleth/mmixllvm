#ifndef BASIC_OS_INCLUDED
#define BASIC_OS_INCLUDED

#include "mmixdef.h"

namespace MmixLlvm {

class ExecutionContext;

class SyscallHandler {
public:
  virtual void handleSyscall(ExecutionContext& ctx, MXUTetra opc) = 0;
  virtual ~SyscallHandler() = default;
};

}

#endif