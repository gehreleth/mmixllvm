#ifndef JIT_LLVM_SESSION_H_INCLUDED
#define JIT_LLVM_SESSION_H_INCLUDED

class llvm::LLVMContext;
class llvm::TargetMachine;
class llvm::DataLayout;

namespace MmixLlvm {
namespace jit {
namespace JitPrivate {

class LlvmSessionContext {
public:
  LlvmSessionContext(llvm::LLVMContext& llvmContext,
                     std::unique_ptr<llvm::TargetMachine> targetMachine,
                     llvm::DataLayout dataLayout);
  
  llvm::LLVMContext& lctx() const;
  llvm::TargetMachine& targetMachine() const;
  const llvm::DataLayout& dataLayout() const;

private:
  llvm::LLVMContext& llvmContext_;
  std::unique_ptr<llvm::TargetMachine> targetMachine_;
  llvm::DataLayout dataLayout_;
};

}
}
}

#endif