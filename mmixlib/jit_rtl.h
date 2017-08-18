#ifndef JIT_RTL_H_INCLUDED
#define JIT_RTL_H_INCLUDED

#include <llvm/ExecutionEngine/Orc/JITSymbol.h>

namespace MmixLlvm {
namespace jit {
namespace JitPrivate {

extern const char* READ_REG_UINT;
extern const char* READ_REG_FP;
extern const char* STORE_REG_UINT;
extern const char* STORE_REG_FP;
extern const char* STORE_MEM_UINT8;
extern const char* STORE_MEM_UINT16;
extern const char* STORE_MEM_UINT32;
extern const char* STORE_MEM_UINT64;
extern const char* STORE_MEM_FP64;
extern const char* STORE_MEM_FP32;
extern const char* READ_MEM_UINT8;
extern const char* READ_MEM_UINT16;
extern const char* READ_MEM_UINT32;
extern const char* READ_MEM_UINT64;
extern const char* READ_MEM_FP64;
extern const char* READ_MEM_FP32;

llvm::orc::JITSymbol lookupRtlSymbol(const char* name);

}
}
}

#endif