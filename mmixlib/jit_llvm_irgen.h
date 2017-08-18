#ifndef JIT_LLVM_IRGEN_H_INCLUDED
#define JIT_LLVM_IRGEN_H_INCLUDED

class llvm::Value;
class llvm::Type;
class llvm::Module;

#include "mmixdef.h"

namespace MmixLlvm {
namespace jit {
namespace JitPrivate {

class LlvmSessionContext;

class IRGenContext {
public:
  using IRBuilder = llvm::IRBuilder<>;

  IRGenContext(LlvmSessionContext& session);
  IRGenContext() = delete;
  IRGenContext(const IRGenContext&) = delete;

  void operator=(const IRGenContext&) = delete;
  void operator=(IRGenContext&&) = delete;

  LlvmSessionContext& getSession();
  llvm::Module& module() const;
  std::unique_ptr<llvm::Module> takeM();
  IRBuilder& irBuilder();
  llvm::Value* cookie() const;
  void setCookie(llvm::Value* cookie);
  llvm::Type* getUByteTy() const;
  llvm::Type* getUWydeTy() const;
  llvm::Type* getUTetraTy() const;
  llvm::Type* getUOctaTy() const;
  llvm::Type* getFpTy() const;
  llvm::Type* getFpsTy() const;
  llvm::Type* getPUByteTy() const;
  llvm::Type* getPUWydeTy() const;
  llvm::Type* getPUTetraTy() const;
  llvm::Type* getPUOctaTy() const;
  llvm::Type* getVoidTy() const;
  llvm::Value* getByteC(MXUByte arg) const;
  llvm::Function* getRtlFunc(const char* name);
  llvm::Function* jitFunctionTemplate(const char* name);
  llvm::BasicBlock* createBB(const char* name, llvm::Function* function);
  llvm::Value* emitReadRegUI64(MXUByte reg);
  void emitStoreRegUI64(MXUByte reg, llvm::Value* val);
  llvm::Value* emitReadMemUI8(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemSI8(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemUI16(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemSI16(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemUI32(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemSI32(llvm::Value* base, llvm::Value* offset);
  llvm::Value* emitReadMemI64(llvm::Value* base, llvm::Value* offset);
private:
  static llvm::Function* make_READ_REG_UINT(IRGenContext& handback);
  static llvm::Function* make_READ_REG_FP(IRGenContext& handback);
  static llvm::Function* make_STORE_REG_UINT(IRGenContext& handback);
  static llvm::Function* make_STORE_REG_FP(IRGenContext& handback);
  static llvm::Function* make_STORE_MEM_UINT8(IRGenContext& handback);
  static llvm::Function* make_STORE_MEM_UINT16(IRGenContext& handback);
  static llvm::Function* make_STORE_MEM_UINT32(IRGenContext& handback);
  static llvm::Function* make_STORE_MEM_UINT64(IRGenContext& handback);
  static llvm::Function* make_READ_MEM_UINT8(IRGenContext& handback);
  static llvm::Function* make_READ_MEM_UINT16(IRGenContext& handback);
  static llvm::Function* make_READ_MEM_UINT32(IRGenContext& handback);
  static llvm::Function* make_READ_MEM_UINT64(IRGenContext& handback);

  LlvmSessionContext &session_;
  std::unique_ptr<llvm::Module> module_;
  IRBuilder builder_;
  llvm::Value* cookie_;
  std::unordered_map<std::string, llvm::Function*> cachedRtlFunctions_;
};

}
}
}


#endif