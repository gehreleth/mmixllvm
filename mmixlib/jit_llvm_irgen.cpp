#include "stdafx.h"
#include "misc.h"
#include "jit_llvm_irgen.h"
#include "jit_llvm_session.h"
#include "jit_rtl.h"

using llvm::Function;
using llvm::Module;
using llvm::Type;
using llvm::Value;
using llvm::FunctionType;
using llvm::ConstantInt;
using llvm::BasicBlock;

using namespace MmixLlvm;
using namespace MmixLlvm::jit::JitPrivate;

IRGenContext::IRGenContext(LlvmSessionContext& session)
  : session_{ session }
  , module_{ std::make_unique<Module>(genUniq("jit_module_"), session.lctx()) }
  , builder_{ session.lctx() }
{
  module_->setDataLayout(session_.targetMachine().createDataLayout());
}

LlvmSessionContext& IRGenContext::getSession() { 
  return session_; 
}

Module& IRGenContext::module() const { 
  return *module_; 
}

std::unique_ptr<Module> IRGenContext::takeM() {
  return std::move(module_); 
}

IRGenContext::IRBuilder& IRGenContext::irBuilder() { 
  return builder_; 
}

Value* IRGenContext::cookie() const { 
  return cookie_; 
}

void IRGenContext::setCookie(Value* cookie) { 
  cookie_ = cookie; 
}

Type* IRGenContext::getUByteTy() const {
  return Type::getInt8Ty(session_.lctx()); 
}

Type* IRGenContext::getUWydeTy() const {
  return Type::getInt16Ty(session_.lctx());
}

Type* IRGenContext::getUTetraTy() const {
  return Type::getInt32Ty(session_.lctx()); 
}

Type* IRGenContext::getUOctaTy() const { 
  return Type::getInt64Ty(session_.lctx());
}

Type* IRGenContext::getFpTy() const { 
  return Type::getDoubleTy(session_.lctx()); 
}

Type* IRGenContext::getFpsTy() const { 
  return Type::getFloatTy(session_.lctx()); 
}

Type* IRGenContext::getPUByteTy() const {
  return Type::getInt8Ty(session_.lctx())->getPointerTo(); 
}

Type* IRGenContext::getPUWydeTy() const {
  return Type::getInt16Ty(session_.lctx())->getPointerTo(); 
}

Type* IRGenContext::getPUTetraTy() const {
  return Type::getInt32Ty(session_.lctx())->getPointerTo(); 
}

Type* IRGenContext::getPUOctaTy() const { 
  return Type::getInt64Ty(session_.lctx())->getPointerTo(); 
}

Type* IRGenContext::getVoidTy() const { 
  return Type::getVoidTy(session_.lctx()); 
}

Value* IRGenContext::getByteC(MXUByte arg) const {
  return ConstantInt::get(getUByteTy(), arg); 
};

Function* IRGenContext::jitFunctionTemplate(const char* fnName)
{
  std::vector<Type*> params = { getPUByteTy() };
  auto fnType = FunctionType::get(getVoidTy(), params, false);
  auto retVal = Function::Create(fnType, Function::ExternalLinkage, fnName, module_.get());
  auto argItr = retVal->arg_begin();
  auto cookie = &(*argItr);
  cookie->setName("cookie");
  setCookie(cookie);
  return retVal;
}

BasicBlock* IRGenContext::createBB(const char* name, Function* function)
{
  return BasicBlock::Create(session_.lctx(), name, function);
}

Function* IRGenContext::getRtlFunc(const char* name)
{
  auto itr = cachedRtlFunctions_.find(name);
  if (itr != cachedRtlFunctions_.end()) {
    return itr->second;
  } else {
    static struct RtlFuncFactoryImpl {
      using RtlFuncFactory = std::function<Function*(IRGenContext&)>;
      using RtlFuncFactoryMap = std::unordered_map<std::string, RtlFuncFactory>;
      RtlFuncFactoryMap rffm_ = {
        { JitPrivate::READ_REG_UINT, &make_READ_REG_UINT },
        { JitPrivate::READ_REG_FP, &make_READ_REG_FP },
        { JitPrivate::STORE_REG_UINT, &make_STORE_REG_UINT },
        { JitPrivate::STORE_REG_FP, &make_STORE_REG_FP },
        { JitPrivate::STORE_MEM_UINT8, &make_STORE_MEM_UINT8 },
        { JitPrivate::STORE_MEM_UINT16, &make_STORE_MEM_UINT16 },
        { JitPrivate::STORE_MEM_UINT32, &make_STORE_MEM_UINT32 },
        { JitPrivate::STORE_MEM_UINT64, &make_STORE_MEM_UINT64 },
        { JitPrivate::READ_MEM_UINT8, &make_READ_MEM_UINT8 },
        { JitPrivate::READ_MEM_UINT16, &make_READ_MEM_UINT16 },
        { JitPrivate::READ_MEM_UINT32, &make_READ_MEM_UINT32 },
        { JitPrivate::READ_MEM_UINT64, &make_READ_MEM_UINT64 },
      };

      Function* makeRtlFunction(IRGenContext& hb, const char* name)
      {
        auto itr = rffm_.find(name);
        if (itr != rffm_.end()) {
          return itr->second(hb);
        } else {
          return nullptr;
        }
      }
    } rtlFuncFactory;
    auto newInst = rtlFuncFactory.makeRtlFunction(*this, name);
    if (newInst != nullptr) {
      cachedRtlFunctions_[name] = newInst;
    }
    return newInst;
  }
}

Value* IRGenContext::emitReadRegUI64(MXUByte reg)
{
  std::vector<Value*> args = { cookie(), getByteC(reg) };
  return irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_REG_UINT), args);
}

void IRGenContext::emitStoreRegUI64(MXUByte reg, Value* val)
{
  std::vector<Value*> args = { cookie(), getByteC(reg) };
  irBuilder().CreateCall(getRtlFunc(JitPrivate::STORE_REG_UINT), args);
}

Value* IRGenContext::emitReadMemUI8(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::ZExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT8), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemSI8(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::SExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT8), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemUI16(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::ZExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT16), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemSI16(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::SExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT16), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemUI32(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::ZExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT32), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemSI32(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCast(llvm::Instruction::CastOps::SExt,
                                irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT32), args),
                                getUOctaTy());
}

Value* IRGenContext::emitReadMemI64(Value* base, Value* offset)
{
  std::vector<Value*> args = { cookie(), base, offset };
  return irBuilder().CreateCall(getRtlFunc(JitPrivate::READ_MEM_UINT64), args);
}

Function* IRGenContext::make_READ_REG_UINT(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(), handback.getUByteTy() };
  return Function::Create(FunctionType::get(handback.getUOctaTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_REG_UINT, handback.module_.get());
}

Function* IRGenContext::make_READ_REG_FP(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(), handback.getUByteTy() };
  return Function::Create(FunctionType::get(handback.getFpTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_REG_FP, handback.module_.get());
}

Function* IRGenContext::make_STORE_REG_UINT(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUByteTy(),
    handback.getUOctaTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_REG_UINT, handback.module_.get());
}

Function* IRGenContext::make_STORE_REG_FP(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUByteTy(),
    handback.getFpTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_REG_FP, handback.module_.get());
}

Function* IRGenContext::make_STORE_MEM_UINT8(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy(),
    handback.getUByteTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_MEM_UINT8, handback.module_.get());
}

Function* IRGenContext::make_STORE_MEM_UINT16(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy(),
    handback.getUWydeTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_MEM_UINT16, handback.module_.get());
}

Function* IRGenContext::make_STORE_MEM_UINT32(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy(),
    handback.getUTetraTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_MEM_UINT32, handback.module_.get());
}

Function* IRGenContext::make_STORE_MEM_UINT64(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy(),
    handback.getUTetraTy()
  };
  return Function::Create(FunctionType::get(handback.getVoidTy(), params, false),
    Function::ExternalLinkage, JitPrivate::STORE_MEM_UINT64, handback.module_.get());
}

Function* IRGenContext::make_READ_MEM_UINT8(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy()
  };
  return Function::Create(FunctionType::get(handback.getUByteTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_MEM_UINT8, handback.module_.get());
}

Function* IRGenContext::make_READ_MEM_UINT16(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy()
  };
  return Function::Create(FunctionType::get(handback.getUWydeTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_MEM_UINT8, handback.module_.get());
}

Function* IRGenContext::make_READ_MEM_UINT32(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy()
  };
  return Function::Create(FunctionType::get(handback.getUTetraTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_MEM_UINT8, handback.module_.get());
}

Function* IRGenContext::make_READ_MEM_UINT64(IRGenContext& handback)
{
  std::vector<Type*> params = { handback.getPUByteTy(),
    handback.getUOctaTy(),
    handback.getUOctaTy()
  };
  return Function::Create(FunctionType::get(handback.getUOctaTy(), params, false),
    Function::ExternalLinkage, JitPrivate::READ_MEM_UINT8, handback.module_.get());
}
