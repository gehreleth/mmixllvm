#ifndef COST_GRAPH_H_INCLUDED
#define COST_GRAPH_H_INCLUDED

#include <memory>
#include <chrono>
#include <ostream>
#include "mmixdef.h"
#include "stats_collector.h"
#include <llvm/ADT/SmallVector.h>

namespace MmixLlvm {

namespace cg {

struct Symbol {
  virtual bool isUndef() const = 0;
  virtual const char* value() const = 0;
  virtual ~Symbol() = 0;
  static const Symbol& UndefSymbol();
};

class CostGraphNode;

class CostGraph : public StatsCollector {
public:
  CostGraph();
  ~CostGraph() override;

  CostGraph(const CostGraph&) = delete;
  CostGraph& operator=(const CostGraph&) = delete;
  CostGraph(CostGraph&&) = delete;
  CostGraph& operator=(CostGraph&&) = delete;

  enum class ConnectionType { Walk, Jump, CondJump, Call, Pop, Trip, TripResume, Trap, TrapResume };

  using NodeType = CostGraphNode;

  using Cost = std::chrono::high_resolution_clock::duration;

  using WholeStorageT = std::vector< CostGraphNode* >;
  using ChildStorageT = llvm::SmallVector<CostGraphNode*, 5>;

  using nodes_iterator = WholeStorageT::iterator;
  using child_iterator = ChildStorageT::iterator;

  CostGraphNode* entry() const;

  nodes_iterator begin() const;
  nodes_iterator end() const;
  child_iterator child_begin(NodeType* parent) const;
  child_iterator child_end(NodeType* parent) const;
  size_t size() const;

  void traceInit(MXUOcta to) override;
  void traceJump(MXUOcta from, MXUOcta to) override;
  void traceCondJump(MXUOcta from, MXUOcta to) override;
  void traceCall(MXUOcta from, MXUOcta to) override;
  void tracePop(MXUOcta from, MXUOcta to) override;
  void traceTrip(MXUOcta from, MXUOcta to) override;
  void traceTrap(MXUOcta from, MXUOcta to) override;
  void traceResumeFromTrip(MXUOcta from, MXUOcta to) override;
  void traceResumeFromTrap(MXUOcta from, MXUOcta to) override;

  void registerSymbol(const char* symbol, MXUOcta equiv);

  void clearStats();
#ifdef _DEBUG
  void dump(std::ostream& os);
#endif
private:
  static void initStatic();
  void ensureIteratorValidity() const;
  void ensureChildIteratorValidity(NodeType* parent) const;
  class CostGraphGuts;
  std::unique_ptr<CostGraphGuts> guts_;
  class SymtabImpl;
  std::unique_ptr<SymtabImpl> symtab_;
};

class EdgeEnumerator {
public:
  virtual void enumWalkEdge(CostGraph::NodeType* pAdjNode, MXUOcta walkCount) = 0;
  virtual void enumCallEdge(CostGraph::NodeType* pAdjNode, CostGraph::Cost cost) = 0;
  virtual void enumJumpEdge(CostGraph::NodeType* pAdjNode, CostGraph::ConnectionType connType) = 0;
  virtual ~EdgeEnumerator() = default;
};

class CostGraphNode {
public:
  virtual MXUOcta fnAddr() const = 0;
  virtual const Symbol& fnSymbol() const = 0;
  virtual MXUOcta addr() const = 0;
  virtual const Symbol& symbol() const = 0;
  virtual void enumEdges(EdgeEnumerator& enumerator) const = 0;
  virtual ~CostGraphNode() = default;
};

CostGraph::NodeType* findMostHeavyFunctionEntry(const CostGraph& cg);

}

}

#endif