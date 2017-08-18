#ifndef JIT_H_INCLUDED
#define JIT_H_INCLUDED

#include <memory>
#include <functional>
#include "econtext.h"
#include "cost_graph.h"

namespace MmixLlvm {
namespace jit {

class JitContext {
public:
  using CompiledFunction = std::function<void()>;
  JitContext(ExecutionContext& ctx);
  ~JitContext();

  JitContext(const JitContext&) = delete;
  JitContext& operator=(const JitContext&) = delete;
  JitContext(JitContext&&) = delete;
  JitContext& operator=(JitContext&&) = delete;

  CompiledFunction compileFunction(cg::CostGraph& cg, cg::CostGraph::NodeType* entry);
private:
  static void initStatic();
  class JitContextGuts;
  std::unique_ptr<JitContextGuts> guts_;
};

}
}

#endif //MISC_H_INCLUDED