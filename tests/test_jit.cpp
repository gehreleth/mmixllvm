#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>
#include <cost_graph.h>
#include <jit.h>
#include <fstream>


using namespace MmixLlvm;

void testJit(MmixLlvm::ExecutionContext& ctx)
{
  cg::CostGraph cg;
  ctx.registerStatsCollector(&cg);
  struct SaveSyms : public MmoLoaderDestRef {
    cg::CostGraph& cgRef_;
    SaveSyms(MmixLlvm::ExecutionContext& ctx, cg::CostGraph& cg)
      :MmoLoaderDestRef(ctx), cgRef_(cg)
    {}
    
    void setSymbol(const char* symbol, MXUOcta equiv) override
    {
      cgRef_.registerSymbol(symbol, equiv);
    }
  } loader{ctx, cg};

  if (loadMmoFile(loader, L"sc.mmo")) {
    stepInto(ctx, 0x100); ctx.setHalt(false);

    cg::CostGraphNode* entry = nullptr;
    for (auto itr = cg.begin(), end = cg.end(); itr != end; ++itr) {
      if ((*itr)->addr() == 0x200) {
        entry = *itr;
        break;
      }
    }

    assertTrue(entry != nullptr);
#ifdef _DEBUG
    cg.dump(std::ofstream{ "scexecution.gv" });
#endif

    MmixLlvm::jit::JitContext jitContext{ ctx };
    auto compiledFn = jitContext.compileFunction(cg, entry);
    compiledFn();
  } else {
    assertTrue(false, "Can't load MMO");
  }

  ctx.registerStatsCollector(nullptr);
}