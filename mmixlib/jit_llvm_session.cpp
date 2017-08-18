#include "stdafx.h"
#include "jit_llvm_session.h"

using llvm::LLVMContext;
using llvm::TargetMachine;
using llvm::DataLayout;

using namespace MmixLlvm;
using namespace MmixLlvm::jit::JitPrivate;

LlvmSessionContext::LlvmSessionContext(LLVMContext& llvmContext,
                   std::unique_ptr<TargetMachine> targetMachine,
                   DataLayout dataLayout)
  : llvmContext_{ llvmContext }
  , targetMachine_{ std::move(targetMachine) }
  , dataLayout_{ dataLayout }
{}

LLVMContext& LlvmSessionContext::lctx() const {
  return llvmContext_; 
}

TargetMachine& LlvmSessionContext::targetMachine() const {
  return *targetMachine_; 
}

const DataLayout& LlvmSessionContext::dataLayout() const {
  return dataLayout_; 
}
