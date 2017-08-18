#include "stdafx.h"
#include "jit.h"
#include "jit_rtl.h"
#include "jit_llvm_session.h"
#include "jit_llvm_irgen.h"
#include "misc.h"
#include "econtext.h"
#include "stats_collector.h"

using namespace MmixLlvm;
using namespace MmixLlvm::jit;
using namespace MmixLlvm::jit::JitPrivate;

using llvm::Type;
using llvm::Value;
using llvm::FunctionType;
using llvm::Function;
using llvm::BasicBlock;
using llvm::DataLayout;
using llvm::orc::SimpleCompiler;
using ObjLayer = llvm::orc::ObjectLinkingLayer<>;
using CompileLayer = llvm::orc::IRCompileLayer<ObjLayer>;
using llvm::orc::JITSymbol;
using ModuleHandleT = CompileLayer::ModuleSetHandleT;
using llvm::Module;
using llvm::raw_string_ostream;
using llvm::Mangler;
using llvm::RuntimeDyld;
using llvm::SectionMemoryManager;
using llvm::TargetMachine;
using llvm::LLVMContext;
using llvm::EngineBuilder;
using llvm::ConstantInt;

class JitContext::JitContextGuts {
public:
  JitContextGuts(ExecutionContext& econtext,
                 std::unique_ptr<LlvmSessionContext> llvmSession,
                 std::unique_ptr<ObjLayer> objectLayer,
                 std::unique_ptr<CompileLayer> compileLayer)
    : econtext_{ econtext }
    , llvmSession_{ std::move(llvmSession) }
    , objectLayer_{ std::move(objectLayer) }
    , compileLayer_{ std::move(compileLayer) }
  { }
  std::unique_ptr<llvm::Module> makeModule(const std::string& fnName, cg::CostGraph& cg, cg::CostGraph::NodeType* entry);
  JitContext::CompiledFunction compileFunctionImpl(cg::CostGraph& cg, cg::CostGraph::NodeType* entry);
  std::string mangle(const std::string &Name);
  ModuleHandleT addModule(std::unique_ptr<Module> module);
  void removeModule(ModuleHandleT moduleHandle);
  JITSymbol findSymbol(const std::string& name);
  JITSymbol findUnmangledSymbol(const std::string& name);
private:
  ExecutionContext& econtext_;
  std::unique_ptr<LlvmSessionContext> llvmSession_;
  std::unique_ptr<ObjLayer> objectLayer_;
  std::unique_ptr<CompileLayer> compileLayer_;
};

std::unique_ptr<llvm::Module> 
  JitContext::JitContextGuts::makeModule(const std::string& fnName,
                                         cg::CostGraph& cg,
                                         cg::CostGraph::NodeType* entry)
{
  IRGenContext ictx{ *llvmSession_ };
  auto& module = ictx.module();
  auto& builder = ictx.irBuilder();

  auto fn = ictx.jitFunctionTemplate(fnName.c_str());

  auto entryBlock = ictx.createBB("entryBlock", fn);
  builder.SetInsertPoint(entryBlock);

  auto base = ictx.emitReadRegUI64(2);
  auto offset = ictx.emitReadRegUI64(3);
  auto memByte = ictx.emitReadMemSI8(base, offset);

  builder.CreateRetVoid();
  module.dump();
  return ictx.takeM();
}

JitContext::CompiledFunction JitContext::JitContextGuts::compileFunctionImpl(cg::CostGraph& cg,
                                                                             cg::CostGraph::NodeType* entry)
{
  auto fnName = genUniq("jit_fn");
  addModule(makeModule(fnName, cg, entry));
  auto symbol = findUnmangledSymbol(fnName);
  void (*q)(ExecutionContext*) = reinterpret_cast<void(*)(ExecutionContext*)>(symbol.getAddress());
  q(&econtext_);
  return JitContext::CompiledFunction{};
}

ModuleHandleT JitContext::JitContextGuts::addModule(std::unique_ptr<Module> module)
{
  auto externalLookup = [this](const std::string& name)
  {
    auto sym = findSymbol(name);
    return sym
      ? RuntimeDyld::SymbolInfo{ sym.getAddress(), sym.getFlags() }
      : RuntimeDyld::SymbolInfo{ nullptr };
  };
  auto dynlibLookup = [](const std::string&)
  {
    return nullptr;
  };
  auto resolver = llvm::orc::createLambdaResolver(externalLookup, dynlibLookup);
  return compileLayer_->addModuleSet(singletonSet(std::move(module)),
                                     std::make_unique<SectionMemoryManager>(),
                                     std::move(resolver));
}

void JitContext::JitContextGuts::removeModule(ModuleHandleT moduleHandle)
{
  compileLayer_->removeModuleSet(moduleHandle);
}

JITSymbol JitContext::JitContextGuts::findSymbol(const std::string& name)
{
  if (auto sym = JitPrivate::lookupRtlSymbol(name.c_str())) {
    return sym;
  } else {
    return compileLayer_->findSymbol(name, false);
  }
}

JITSymbol JitContext::JitContextGuts::findUnmangledSymbol(const std::string& name)
{
  return findSymbol(mangle(name));
}

std::string JitContext::JitContextGuts::mangle(const std::string& name)
{
  std::string MangledName;
  {
    raw_string_ostream MangledNameStream{ MangledName };
    Mangler::getNameWithPrefix(MangledNameStream, name, llvmSession_->dataLayout());
  }
  return MangledName;
}

void JitContext::initStatic()
{
  static struct InitS {
    InitS()
    {
      llvm::InitializeNativeTarget();
#ifdef _DEBUG
      llvm::InitializeNativeTargetAsmPrinter();
      llvm::InitializeNativeTargetAsmParser();
#endif
    }

    ~InitS()
    {
      llvm::llvm_shutdown();
    }
  } initS;
}

JitContext::CompiledFunction JitContext::compileFunction(cg::CostGraph& cg, cg::CostGraph::NodeType* entry)
{
  return guts_->compileFunctionImpl(cg, entry);
}

JitContext::JitContext(ExecutionContext& econtext)
{
  initStatic();
  llvm::LLVMContext& lctx = llvm::getGlobalContext();
  std::unique_ptr<TargetMachine> targetMachine{ EngineBuilder{}.selectTarget() };
  auto dataLayout = targetMachine->createDataLayout();
  auto lsc = std::make_unique<LlvmSessionContext>(lctx, std::move(targetMachine), dataLayout );
  auto objLayer = std::make_unique<ObjLayer>();
  auto compileLayer = std::make_unique<CompileLayer>(*objLayer, SimpleCompiler{ lsc->targetMachine() });
  guts_ = std::make_unique<JitContextGuts>(econtext, std::move(lsc), std::move(objLayer), std::move(compileLayer));
}

JitContext::~JitContext() { }
