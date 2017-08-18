#include "stdafx.h"
#include "jit_rtl.h"
#include "mmixdef.h"
#include "econtext.h"

using namespace MmixLlvm;

const char* MmixLlvm::jit::JitPrivate::READ_REG_UINT = "_readRegUint";
const char* MmixLlvm::jit::JitPrivate::READ_REG_FP = "_readRegFp";
const char* MmixLlvm::jit::JitPrivate::STORE_REG_UINT = "_storeRegUint";
const char* MmixLlvm::jit::JitPrivate::STORE_REG_FP = "_storeRegFp";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_UINT8 = "_storeMemUint8";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_UINT16 = "_storeMemUint16";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_UINT32 = "_storeMemUint32";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_UINT64 = "_storeMemUint64";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_FP64 = "_storeMemFp";
const char* MmixLlvm::jit::JitPrivate::STORE_MEM_FP32 = "_storeMemFps";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_UINT8 = "_readMemUint8";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_UINT16 = "_readMemUint16";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_UINT32 = "_readMemUint32";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_UINT64 = "_readMemUint64";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_FP64 = "_readMemFp";
const char* MmixLlvm::jit::JitPrivate::READ_MEM_FP32 = "_readMemFps";

namespace {

MXUOcta readRegUint(ExecutionContext* context, MXUByte reg)
{
  return context->readReg<MXUOcta>(reg);
}

MXFp readRegFp(ExecutionContext* context, MXUByte reg)
{
  return context->readReg<MXFp>(reg);
}

void storeRegUint(ExecutionContext* context, MXUByte reg, MXUOcta value)
{
  context->storeReg(reg, value);
}

void storeRegFp(ExecutionContext* context, MXUByte reg, MXFp value)
{
  context->storeReg(reg, value);
}

void storeMemUint8(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXUByte value)
{
  context->storeMem(base, offset, value);
}

void storeMemUint16(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXUWyde value)
{
  context->storeMem(base, offset, value);
}

void storeMemUint32(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXUTetra value)
{
  context->storeMem(base, offset, value);
}

void storeMemUint64(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXUOcta value)
{
  context->storeMem(base, offset, value);
}

void storeMemFp(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXFp value)
{
  context->storeMem(base, offset, value);
}

void storeMemFps(ExecutionContext* context, MXUOcta base, MXUOcta offset, MXFps value)
{
  context->storeMem(base, offset, value);
}

MXUByte readMemUint8(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXUByte>(base, offset);
}

MXUWyde readMemUint16(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXUWyde>(base, offset);
}

MXUTetra readMemUint32(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXUTetra>(base, offset);
}

MXUOcta readMemUint64(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXUOcta>(base, offset);
}

MXFp readMemFp(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXFp>(base, offset);
}

MXFps readMemFps(ExecutionContext* context, MXUOcta base, MXUOcta offset)
{
  return context->readMem<MXFps>(base, offset);
}

std::unique_ptr< std::unordered_map<std::string, void*> > TheRtl;
}

llvm::orc::JITSymbol MmixLlvm::jit::JitPrivate::lookupRtlSymbol(const char * name)
{
  static struct LookupInit {
    LookupInit()
    {
      std::unordered_map<std::string, void*> tmpMap = {
        { MmixLlvm::jit::JitPrivate::READ_REG_UINT, &readRegUint },
        { MmixLlvm::jit::JitPrivate::READ_REG_FP, &readRegFp },
        { MmixLlvm::jit::JitPrivate::STORE_REG_UINT, &storeRegUint },
        { MmixLlvm::jit::JitPrivate::STORE_REG_FP, &storeRegFp },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_UINT8, &storeMemUint8 },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_UINT16, &storeMemUint16 },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_UINT32, &storeMemUint32 },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_UINT64, &storeMemUint64 },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_FP64, &storeMemFp },
        { MmixLlvm::jit::JitPrivate::STORE_MEM_FP32, &storeMemFps },
        { MmixLlvm::jit::JitPrivate::READ_MEM_UINT8, &readMemUint8 },
        { MmixLlvm::jit::JitPrivate::READ_MEM_UINT16, &readMemUint16 },
        { MmixLlvm::jit::JitPrivate::READ_MEM_UINT32, &readMemUint32 },
        { MmixLlvm::jit::JitPrivate::READ_MEM_UINT64, &readMemUint64 },
        { MmixLlvm::jit::JitPrivate::READ_MEM_FP64, &readMemFp },
        { MmixLlvm::jit::JitPrivate::READ_MEM_FP32, &readMemFps },
      };
      auto tmpPointer = std::make_unique< std::unordered_map<std::string, void*> >();
      (*tmpPointer).swap(tmpMap);
      TheRtl = std::move(tmpPointer);
    }
  } lookupInit;
  auto itr = (*TheRtl).find(name);
  if (itr != (*TheRtl).end()) {
    return llvm::orc::JITSymbol{ reinterpret_cast<uint64_t>(itr->second), llvm::JITSymbolFlags::Exported };
  } else {
    return llvm::orc::JITSymbol(nullptr);
  }
}
