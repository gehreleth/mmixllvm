#include "stdafx.h"
#include "cost_graph.h"
#include "errorhandling.h"
#include <stack>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <functional>

using namespace MmixLlvm;
using namespace MmixLlvm::cg;

using Timepoint = std::chrono::high_resolution_clock::time_point;

namespace {
class UndefSymbol : public Symbol {
  bool isUndef() const override
  {
    return true;
  }

  const char* value() const override
  {
    assert(false && "Undefined symbol");
    return "<Unknown>";
  }
} undefSymbol;

class SymbolImpl : public Symbol {
  std::string value_;
  bool illegal_;
public:
  SymbolImpl()
    : illegal_(true)
  {}

  explicit SymbolImpl(const char* value)
    : value_(value)
    , illegal_(false)
  {};

  SymbolImpl(const SymbolImpl&) = default;
  SymbolImpl& operator=(const SymbolImpl&) = default;
  SymbolImpl(SymbolImpl&&) = default;
  SymbolImpl& operator=(SymbolImpl&&) = default;

  ~SymbolImpl() override = default;

  bool isUndef() const override
  {
    assert(!illegal_ && "Undefined symbol");
    return false;
  }

  const char* value() const override
  {
    assert(!illegal_ && "Uninitialized SymbolImpl object");
    return value_.c_str();
  }
};

struct Edge {
  Edge()
    :initialized_(false)
  {
    memset(&opt_, 0, sizeof(opt_));
  }
  Edge(const Edge&) = default;
  Edge& operator=(const Edge&) = default;
  Edge(Edge&&) = default;
  Edge& operator=(Edge&&) = default;

  static Edge makeWalk(MXUOcta link)
  {
    Edge retVal;
    retVal.connType_ = CostGraph::ConnectionType::Walk;
    retVal.link_ = link;
    retVal.opt_.walkCount_ = 1;
    retVal.initialized_ = true;
    return retVal;
  }

  static Edge makeJump(MXUOcta link, CostGraph::ConnectionType connType)
  {
    if (connType != CostGraph::ConnectionType::Walk) {
      Edge retVal;
      retVal.connType_ = connType;
      if (connType == CostGraph::ConnectionType::Call) {
        new(&retVal.opt_.cost_) CostGraph::Cost{ 0 }; // XXX. Let's call 
                                // supplied constructor for it.
      }
      retVal.link_ = link;
      retVal.initialized_ = true;
      return retVal;
    } else {
      assert(false && "illegal use of Edge::makeJump()");
      return Edge{};
    }
  }

  MXUOcta link() const
  {
    assert(initialized_ && "Edge instance is not initialized");
    return link_;
  }

  CostGraph::ConnectionType connType() const
  {
    assert(initialized_ && "Edge instance is not initialized");
    return connType_;
  }

  CostGraph::Cost callCost() const
  {
    assert(initialized_ && "Edge instance is not initialized");
    if (connType_ == CostGraph::ConnectionType::Call) {
      return opt_.cost_;
    } else {
      assert(false && "callCost() for non-call edge");
      return CostGraph::Cost{ ~0 };
    }
  }

  void incCallCost(CostGraph::Cost amount)
  {
    assert(initialized_ && "Edge instance is not initialized");
    if (connType_ == CostGraph::ConnectionType::Call) {
      opt_.cost_ += amount;
    } else {
      assert(false && "incCallCost() for non-call edge");
    }
  }

  MXUOcta walkCount() const
  {
    assert(initialized_ && "Edge instance is not initialized");
    if (connType_ == CostGraph::ConnectionType::Walk) {
      return opt_.walkCount_;
    } else {
      assert(false && "walkCount() for non-walk edge");
      return ~0;
    }
  }

  void incWalkCount()
  {
    assert(initialized_ && "Edge instance is not initialized");
    if (connType_ == CostGraph::ConnectionType::Walk) {
      ++opt_.walkCount_;
    } else {
      assert(false && "incWalkCount() for non-walk edge");
    }
  }
private:
  MXUOcta link_;
  CostGraph::ConnectionType connType_;
  union {
    CostGraph::Cost cost_;
    MXUOcta walkCount_;
  } opt_;
  bool initialized_;
};

}

using EdgeArr = llvm::SmallVector<Edge, 5>;

namespace {
class CostGraphNodeImpl : public CostGraphNode {
public:
  using LazyResolveChild = std::function< CostGraph::NodeType*(const CostGraph*, MXUOcta) >;
  using LazyResolveSymbol = std::function< const Symbol&(const CostGraph*, MXUOcta) >;
  static LazyResolveChild lazyResolveChild_s;
  static LazyResolveSymbol lazyResolveSymbol_s;
private:
  const CostGraph* outer_;
  const MXUOcta fnAddr_;
  const MXUOcta addr_;
  const EdgeArr edgeArr_;
public:
  CostGraphNodeImpl(const CostGraph* outer, MXUOcta fnAddr, MXUOcta addr, const EdgeArr& edgeArr)
    : outer_(outer)
    , fnAddr_(fnAddr)
    , addr_(addr)
    , edgeArr_(edgeArr)
  {}

  MXUOcta fnAddr() const override
  {
    return fnAddr_;
  }

  const Symbol& fnSymbol() const override
  {
    return lazyResolveSymbol_s(outer_, fnAddr_);
  }

  MXUOcta addr() const override
  {
    return addr_;
  }

  const Symbol& symbol() const override
  {
    return lazyResolveSymbol_s(outer_, addr_);
  }

  void enumEdges(EdgeEnumerator& enumerator) const override
  {
    for (auto& edge : edgeArr_) {
      CostGraph::NodeType* child = lazyResolveChild_s(outer_, edge.link());
      switch (edge.connType()) {
        case CostGraph::ConnectionType::Walk:
          enumerator.enumWalkEdge(child, edge.walkCount());
          break;
        case CostGraph::ConnectionType::Call:
          enumerator.enumCallEdge(child, edge.callCost());
          break;
        default:
          enumerator.enumJumpEdge(child, edge.connType());
          break;
      }
    }
  }
};

CostGraphNodeImpl::LazyResolveChild CostGraphNodeImpl::lazyResolveChild_s;
CostGraphNodeImpl::LazyResolveSymbol CostGraphNodeImpl::lazyResolveSymbol_s;

struct TemporaryObjects {
  std::vector< CostGraphNodeImpl > nodesMemStorage_;
  std::vector< CostGraphNode* > whole_;
  std::unordered_map< CostGraphNode*, llvm::SmallVector<CostGraphNode*, 5> > childSets_;
  std::unordered_map< MXUOcta, CostGraphNode* > nodeLookup_;
  ~TemporaryObjects() = default;
};

struct CallTraceRecord {
  CallTraceRecord(Timepoint fnEnterTimepoint, MXUOcta callingInstructionAddress, MXUOcta callee)
    : fnEnterTimepoint_(fnEnterTimepoint)
    , callingInstructionAddress_(callingInstructionAddress)
    , callee_(callee)
  {}

  CallTraceRecord(const CallTraceRecord&) = default;
  CallTraceRecord(CallTraceRecord&&) = default;

  CallTraceRecord& operator=(const CallTraceRecord&) = default;
  CallTraceRecord& operator=(CallTraceRecord&&) = default;

  ~CallTraceRecord() = default;

  Timepoint fnEnterTimepoint() const
  {
    return fnEnterTimepoint_;
  }

  MXUOcta callingInstructionAddress() const
  {
    return callingInstructionAddress_;
  }

  MXUOcta callee() const
  {
    return callee_;
  }
private:
  Timepoint fnEnterTimepoint_;
  MXUOcta callingInstructionAddress_;
  MXUOcta callee_;
};
}

class CostGraph::CostGraphGuts {
public:
  struct AddressPairHash {
    std::size_t operator()(const std::tuple<MXUOcta, MXUOcta>& arg) const
    {
      // these values are always multiplies of four, so we drop two lest significant bits
      return (std::hash<MXUOcta>()(std::get<0>(arg) >> 2) * 443) +
        std::hash<MXUOcta>()(std::get<1>(arg) >> 2);
    }
  };
  MXUOcta prevTarget_ = 0;
  MXUOcta entry_ = 0;
  std::unordered_map<std::tuple<MXUOcta, MXUOcta>, EdgeArr, AddressPairHash> graph_;
  std::stack<CallTraceRecord> stackTrace_;
  std::unique_ptr<TemporaryObjects> tempObjs_;
  void traceExecution(MXUOcta from, MXUOcta to, ConnectionType connType);
  void addOrUpdateWalkEdge(MXUOcta fnAddress, MXUOcta walkEdgeBegin, MXUOcta walkEdgeEnd);
  void addOrUpdateJumpEdge(MXUOcta fnAddress, MXUOcta jumpEdgeBegin, MXUOcta jumpEdgeEnd, ConnectionType connType);
  void incFnCost(MXUOcta callingFunctionEntry, MXUOcta callInstructionAddr, MXUOcta callee, Cost cost);
  ~CostGraphGuts() = default;
};

void CostGraph::CostGraphGuts::traceExecution(MXUOcta from, MXUOcta to, ConnectionType connType)
{
  tempObjs_.reset(); // All graph accessors are invalidated here 
  if (graph_.empty()) {
    entry_ = from;
    prevTarget_ = from;
  }
  MXUOcta walkEdgeBegin = prevTarget_;
  MXUOcta walkEdgeEnd = from;
  MXUOcta jumpEdgeBegin = from;

  MXUOcta currentFunctionEntry = entry_;
  if (!stackTrace_.empty()) {
    currentFunctionEntry = stackTrace_.top().callee();
  }

  switch (connType) {
    case ConnectionType::Call:
      stackTrace_.push(CallTraceRecord{ std::chrono::high_resolution_clock::now(), from, to });
      break;
    case ConnectionType::Pop:
      if (!stackTrace_.empty()) {
        CallTraceRecord r = stackTrace_.top();
        stackTrace_.pop();
        if (to == r.callingInstructionAddress() + 4) { // Usual pop instruction leads
// to the next tetra after call
          const Cost fnCost = std::chrono::high_resolution_clock::now() - r.fnEnterTimepoint();
          MXUOcta outerFunctionEntry = entry_;
          if (!stackTrace_.empty()) { // Extract entry to the function which issued the call
// to the function we're currently popping from from
// the top of our internal stack trace
            outerFunctionEntry = stackTrace_.top().callee();
          }
          // edge[v[calling_function:call_instruction]->v[this_function:entry_instruction]] <- updated cost
          incFnCost(outerFunctionEntry, r.callingInstructionAddress(), r.callee(), fnCost);
          to = r.callingInstructionAddress(); // eliminate redundant & unnecessary 
                            // "next instruction after the call" node
                                              // make use of the call instruction node instead.
        }
      }
      break;
    default:
      break;
  }

  MXUOcta jumpEdgeEnd = to;
  prevTarget_ = to;

  if (walkEdgeBegin != walkEdgeEnd) {
    // Make edge[vertice[previous jump/call/whatever target]->[this jump/call/whatever instruction]]
    // Init or update weight of this sequence of non-jump instructions
    addOrUpdateWalkEdge(currentFunctionEntry, walkEdgeBegin, walkEdgeEnd);
  }
  // Make edge[vertice[this jump/call/whatever instruction]->[target]]
  addOrUpdateJumpEdge(currentFunctionEntry, jumpEdgeBegin, jumpEdgeEnd, connType);
}

void CostGraph::CostGraphGuts::addOrUpdateWalkEdge(MXUOcta fnAddress, MXUOcta walkEdgeBegin, MXUOcta walkEdgeEnd)
{
  auto key = std::make_tuple(fnAddress, walkEdgeBegin);
  auto itr = graph_.find(key);
  if (itr != graph_.end()) {
    auto& edges = itr->second;
    auto edgeItr = std::find_if(edges.begin(), edges.end(), [walkEdgeEnd](auto edge)
    {
      return edge.connType() == ConnectionType::Walk && edge.link() == walkEdgeEnd;
    });
    if (edgeItr != edges.end()) {
      edgeItr->incWalkCount();
    } else {
      edges.emplace_back(Edge::makeWalk(walkEdgeEnd));
    }
  } else {
    graph_.emplace(key, EdgeArr{ Edge::makeWalk(walkEdgeEnd) });
  }
}

void CostGraph::CostGraphGuts::addOrUpdateJumpEdge(MXUOcta fnAddress, MXUOcta jumpEdgeBegin, MXUOcta jumpEdgeEnd, ConnectionType connType)
{
  auto key = std::make_tuple(fnAddress, jumpEdgeBegin);
  auto itr = graph_.find(key);
  if (itr != graph_.end()) {
    auto& edges = itr->second;
    auto edgeItr = std::find_if(edges.begin(), edges.end(), [jumpEdgeEnd, connType](auto edge)
    {
      return edge.connType() == connType && edge.link() == jumpEdgeEnd;
    });
    if (edgeItr == edges.end()) {
      edges.emplace_back(Edge::makeJump(jumpEdgeEnd, connType));
    }
  } else {
    graph_.emplace(key, EdgeArr{ Edge::makeJump(jumpEdgeEnd, connType) });
  }
}

void CostGraph::CostGraphGuts::incFnCost(MXUOcta callingFunctionEntry, MXUOcta callInstructionAddr, MXUOcta callee, Cost cost)
{
  auto itr = graph_.find(std::make_tuple(callingFunctionEntry, callInstructionAddr));
  if (itr != graph_.end()) {
    auto& edges = itr->second;
    auto edgeItr = std::find_if(edges.begin(), edges.end(), [callee](auto edge)
    {
      return edge.connType() == ConnectionType::Call && edge.link() == callee;
    });
    if (edgeItr != edges.end()) {
      edgeItr->incCallCost(cost);
    }
  }
}

Symbol::~Symbol() {}

const Symbol& Symbol::UndefSymbol()
{
  return undefSymbol;
};

class CostGraph::SymtabImpl {
  std::unordered_map<MXUOcta, SymbolImpl> symbols_;
public:
  void registerEquiv(const char* symbol, MXUOcta equiv);
  const Symbol& getSymbol(MXUOcta val) const;
  ~SymtabImpl() = default;
};

void CostGraph::SymtabImpl::registerEquiv(const char* symbol, MXUOcta equiv)
{
  symbols_[equiv] = SymbolImpl{ symbol };
}

const Symbol& CostGraph::SymtabImpl::getSymbol(MXUOcta val) const
{
  auto itr = symbols_.find(val);
  if (itr != symbols_.end()) {
    return itr->second;
  } else {
    return Symbol::UndefSymbol();
  }
}

CostGraphNode* CostGraph::entry() const
{
  ensureIteratorValidity();
  return guts_->tempObjs_->nodeLookup_[guts_->entry_];
}

CostGraph::nodes_iterator CostGraph::begin() const
{
  ensureIteratorValidity();
  return guts_->tempObjs_->whole_.begin();
}

CostGraph::nodes_iterator CostGraph::end() const
{
  ensureIteratorValidity();
  return guts_->tempObjs_->whole_.end();
}

CostGraph::child_iterator CostGraph::child_begin(NodeType* parent) const
{
  ensureChildIteratorValidity(parent);
  return guts_->tempObjs_->childSets_[parent].begin();
}

CostGraph::child_iterator CostGraph::child_end(NodeType* parent) const
{
  ensureChildIteratorValidity(parent);
  return guts_->tempObjs_->childSets_[parent].end();
}

size_t CostGraph::size() const
{
  ensureIteratorValidity();
  return guts_->tempObjs_->whole_.size();
}

void CostGraph::traceInit(MXUOcta to)
{
  guts_->prevTarget_ = to;
  guts_->traceExecution(to, to, ConnectionType::Jump);
}

void CostGraph::traceJump(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::Jump);
}

void CostGraph::traceCondJump(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::CondJump);
}

void CostGraph::traceCall(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::Call);
}

void CostGraph::tracePop(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::Pop);
}

void CostGraph::traceTrip(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::Trip);
}

void CostGraph::traceTrap(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::Trap);
}

void CostGraph::traceResumeFromTrip(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::TripResume);
}

void CostGraph::traceResumeFromTrap(MXUOcta from, MXUOcta to)
{
  guts_->traceExecution(from, to, ConnectionType::TrapResume);
}

void CostGraph::initStatic()
{
  static class InitCgNodeStatic {
  public:
    InitCgNodeStatic()
    {
      CostGraphNodeImpl::lazyResolveChild_s = [](const CostGraph* outer, MXUOcta addr) -> auto {
        return outer->guts_->tempObjs_->nodeLookup_[addr];
      };

      CostGraphNodeImpl::lazyResolveSymbol_s = [](const CostGraph* outer, MXUOcta addr) -> const Symbol&{
        return outer->symtab_->getSymbol(addr);
      };
    }
  } icgns;
}

void CostGraph::ensureIteratorValidity() const
{
  if (!guts_->tempObjs_) {
    guts_->tempObjs_ = std::make_unique<TemporaryObjects>();
    auto& mg = guts_->tempObjs_->nodesMemStorage_;
    for (auto& q : guts_->graph_) {
      MXUOcta functionAddress = std::get<0>(q.first);
      MXUOcta nodeAddress = std::get<1>(q.first);
      const EdgeArr& children = q.second;
      mg.emplace_back(CostGraphNodeImpl{ this, functionAddress, nodeAddress, children });
    }
    for (auto& q : mg) {
      CostGraphNode *nodePtr = &q;
      guts_->tempObjs_->whole_.push_back(nodePtr);
      guts_->tempObjs_->nodeLookup_[q.addr()] = nodePtr;
    }
  }
}

void CostGraph::ensureChildIteratorValidity(NodeType* parent) const
{
  ensureIteratorValidity();
  auto& csets = guts_->tempObjs_->childSets_;
  if (csets.find(parent) == csets.end()) {
    struct EnumC : public EdgeEnumerator {
      ChildStorageT children_;

      void enumWalkEdge(CostGraph::NodeType* pAdjNode, MXUOcta) override
      {
        children_.push_back(pAdjNode);
      }

      void enumCallEdge(CostGraph::NodeType* pAdjNode, CostGraph::Cost) override
      {
        children_.push_back(pAdjNode);
      }

      void enumJumpEdge(CostGraph::NodeType* pAdjNode, CostGraph::ConnectionType) override
      {
        children_.push_back(pAdjNode);
      }
    } enumC;
    parent->enumEdges(enumC);
    csets[parent].swap(enumC.children_);
  }
}

void CostGraph::registerSymbol(const char* symbol, MXUOcta equiv)
{
  symtab_->registerEquiv(symbol, equiv);
}

CostGraph::CostGraph()
  : guts_(std::make_unique<CostGraph::CostGraphGuts>())
  , symtab_(std::make_unique<CostGraph::SymtabImpl>())
{
  initStatic();
}

CostGraph::~CostGraph()
{}

void CostGraph::clearStats()
{
  guts_ = std::make_unique<CostGraph::CostGraphGuts>();
}

#ifdef _DEBUG
namespace {
template<typename T> struct _2double {
  static double calc(T arg);
};

template<typename T> double _2double<T>::calc(T arg) {
  return double(arg);
}

template<> double _2double<MmixLlvm::cg::CostGraph::Cost>::calc(MmixLlvm::cg::CostGraph::Cost arg)
{
  using ddur = std::chrono::duration<double>;
  return std::chrono::duration_cast<ddur>(arg).count();
}

template<typename T> int getWidth(T val, T maxVal)
{
  double c1 = _2double<T>::calc(val);
  double c2 = _2double<T>::calc(maxVal);
  return static_cast<int>(1. + 29. * std::pow(c1 / c2, 2.));
}

std::string formatFunctionCap(MmixLlvm::cg::CostGraph::NodeType* pNode)
{
  std::stringstream ss;
  if (!pNode->symbol().isUndef()) { //known function
    ss << pNode->symbol().value();
  } else {
    ss << std::hex << '#' << pNode->addr();
  }
  return ss.str();
}

std::string formatNodeText(MmixLlvm::cg::CostGraph::NodeType* pNode)
{
  std::stringstream ss;
  ss << '\"';
  if (pNode->addr() == pNode->fnAddr()) { // function entry
    ss << "Entry ";
    if (!pNode->symbol().isUndef()) { //known function
      ss << pNode->symbol().value();
    } else {
      ss << std::hex << '#' << pNode->addr();
    }
  } else {
    if (!pNode->symbol().isUndef()) { //known branch
      ss << pNode->symbol().value();
    } else {
      ss << std::hex << '#' << pNode->addr();
    }
  }
  ss << '\"';
  return ss.str();
}

struct DumpEdgeEnumerator : public EdgeEnumerator {
  using PNode = CostGraphNode*;
  using Cost = CostGraph::Cost;

  std::ostream& ostr_;
  PNode v1_;
  Cost maxCallCost_;
  MXUOcta maxWalkCount_;
  std::vector< std::tuple<PNode, PNode, CostGraph::ConnectionType, CostGraph::Cost> > interFunctionEdges_;

  void enumWalkEdge(PNode adjNode, MXUOcta walkCount) override
  {
    std::string v1 = formatNodeText(v1_);
    std::string v2 = formatNodeText(adjNode);
    ostr_ << v1 << "->" << v2
      << "[ penwidth=" << getWidth(walkCount, maxWalkCount_) << " color=black ]\n";
  }

  void enumCallEdge(PNode adjNode, CostGraph::Cost cost) override
  {
    interFunctionEdges_.emplace_back(std::make_tuple(v1_, adjNode, CostGraph::ConnectionType::Call, cost));
  }

  void enumJumpEdge(PNode adjNode, CostGraph::ConnectionType connType) override
  {
    std::string v1 = formatNodeText(v1_);
    std::string v2 = formatNodeText(adjNode);
    switch (connType) {
      case CostGraph::ConnectionType::Jump:
        ostr_ << v1 << "->" << v2 << "[ penwidth=1 color=blue ]\n";
        break;
      case CostGraph::ConnectionType::CondJump:
        ostr_ << v1 << "->" << v2 << "[ penwidth=1 color=blue style=\"dashed\"]\n";
        break;
      case CostGraph::ConnectionType::Pop:
        interFunctionEdges_.emplace_back(std::make_tuple(v1_, adjNode, CostGraph::ConnectionType::Pop, CostGraph::Cost(0)));
        break;
      case CostGraph::ConnectionType::Trip:
      case CostGraph::ConnectionType::Trap:
        ostr_ << v1 << "->" << v2 << "[ penwidth=1 color=violet ]\n";
        break;
      case CostGraph::ConnectionType::TripResume:
      case CostGraph::ConnectionType::TrapResume:
        ostr_ << v1 << "->" << v2 << "[ penwidth=1 color=violet style=\"dashed\"]\n";
        break;
    }
  }

  void writeInterFunctionEdges()
  {
    for (auto itr = interFunctionEdges_.begin(), end = interFunctionEdges_.end(); itr != end; ++itr) {
      auto v1 = formatNodeText(std::get<0>(*itr));
      auto v2 = formatNodeText(std::get<1>(*itr));
      auto connType = std::get<2>(*itr);
      auto cost = std::get<3>(*itr);
      switch (connType) {
        case CostGraph::ConnectionType::Call:
          ostr_ << v1 << "->" << v2 << "[ penwidth="<< getWidth(cost, maxCallCost_) <<" color=green ]\n";
          break;
        case CostGraph::ConnectionType::Pop:
          ostr_ << v1 << "->" << v2 << "[ penwidth=1 color=green style=\"dashed\"]\n";
          break;
      }
    }
  }

  DumpEdgeEnumerator(std::ostream& ostr)
    :ostr_(ostr)
  {}

  ~DumpEdgeEnumerator() override {}
};

}

void CostGraph::dump(std::ostream& os)
{
  using It = CostGraph::nodes_iterator;
  using ChildIt = CostGraph::child_iterator;
  using PNode = CostGraph::NodeType*;

  struct MaxCostEstimator : public EdgeEnumerator {
    Cost maxCallCost_ = Cost(0);

    MXUOcta maxWalkCount_ = 0;

    void enumWalkEdge(PNode, MXUOcta walkCount) override
    {
      if (walkCount > maxWalkCount_) {
        maxWalkCount_ = walkCount;
      }
    }

    void enumCallEdge(PNode, CostGraph::Cost cost) override
    {
      if (cost > maxCallCost_) {
        maxCallCost_ = cost;
      }
    }

    void enumJumpEdge(PNode, CostGraph::ConnectionType connType) override {}

    ~MaxCostEstimator() override {}
  } mce;


  std::unordered_map<MXUOcta, CostGraphNode* > funcLookup;
  std::unordered_map<MXUOcta, std::unordered_set<CostGraphNode*> > groupedByFunc;

  for (It itr = begin(), end_itr = end(); itr != end_itr; ++itr) {
    if ((*itr)->addr() == (*itr)->fnAddr()) {
      funcLookup[(*itr)->fnAddr()] = *itr;
    }
    (*itr)->enumEdges(q);
    groupedByFunc[(*itr)->fnAddr()].insert(*itr);
  }

  DumpEdgeEnumerator dee{ os };
  dee.maxWalkCount_ = mce.maxWalkCount_;
  dee.maxCallCost_ = mce.maxCallCost_;

  os << "digraph MMIX_code_execution {\n";
  os << "compound = true;\n";
  int c = 0;
  for (auto itr = groupedByFunc.begin(), end = groupedByFunc.end(); itr != end; ++itr) {
    CostGraphNode* funcEntry = funcLookup[itr->first];
    auto n_nodes = itr->second;
    os << "subgraph clusterfunction_"<< ++c << "{\n";
    os << "label = \"function " << formatFunctionCap(funcEntry) << "\";\n";
    for (auto itr0 = n_nodes.begin(), end = n_nodes.end(); itr0 != end; ++itr0) {
      dee.v1_ = *itr0;
      (*itr0)->enumEdges(dee);
    }
    os << "}\n";
  }
  dee.writeInterFunctionEdges();
  os << "}\n";
}
#endif

namespace {
std::tuple<CostGraph::NodeType*, CostGraph::Cost> findMostHeavyCall0(const CostGraph& cg,
                                                                     CostGraph::NodeType* entry,
                                                                     CostGraph::Cost cost);

template<typename Hash, typename OutIt> void enumNestedCalls(const CostGraph& cg,
                                                             CostGraph::NodeType* fnEntry,
                                                             Hash& ctx, OutIt target);

std::tuple<CostGraph::NodeType*, CostGraph::Cost> findMostHeavyCall0(const CostGraph& cg,
                                                                     CostGraph::NodeType* entry,
                                                                     CostGraph::Cost cost)
{
  llvm::SmallPtrSet <CostGraph::NodeType*, 20> visitedFunctions;
  for (;;) {
    llvm::SmallVector< std::tuple<CostGraph::NodeType*, CostGraph::Cost>, 5 > nestedCalls;
    llvm::SmallPtrSet <CostGraph::NodeType*, 20> visitedNodes;
    visitedNodes.insert(entry);
    enumNestedCalls(cg, entry, visitedNodes, std::back_inserter(nestedCalls));
    auto avoidRecursion = std::remove_if(nestedCalls.begin(),
                                         nestedCalls.end(),
                                         [&visitedFunctions](auto arg) -> bool
    {
      return visitedFunctions.count(std::get<0>(arg)) != 0;
    });
    nestedCalls.erase(avoidRecursion, nestedCalls.end());
    if (!nestedCalls.empty()) {
      auto maxIt = std::max_element(nestedCalls.begin(), nestedCalls.end(),
                                    [](auto lho, auto rho) -> bool
      {
        return std::get<1>(lho) < std::get<1>(rho);
      });
      CostGraph::Cost acc{ 0 };
      for (auto nc : nestedCalls) {
        acc += std::get<1>(nc);
      }
      auto ownCost = cost - acc;
      if (std::get<1>(*maxIt) > ownCost) {
        entry = std::get<0>(*maxIt);
        cost = std::get<1>(*maxIt);
        visitedFunctions.insert(entry);
      } else
        break;
    } else
      break;
  }
  return std::make_tuple(entry, cost);
}

template<typename Hash, typename OutIt> struct En : public EdgeEnumerator {
  const CostGraph& cg_;
  Hash& visitedNodes_;
  OutIt target_;

  void enumWalkEdge(CostGraph::NodeType* adj, MXUOcta) override
  {
    if (!visitedNodes_.count(adj)) {
      visitedNodes_.insert(adj);
      enumNestedCalls(cg_, adj, visitedNodes_, target_);
    }
  }

  void enumCallEdge(CostGraph::NodeType* adj, CostGraph::Cost cost) override
  {
    *(target_++) = std::make_tuple(adj, cost);
  }

  void enumJumpEdge(CostGraph::NodeType* adj, CostGraph::ConnectionType connType) override
  {
    if (connType != CostGraph::ConnectionType::Pop && !visitedNodes_.count(adj)) {
      visitedNodes_.insert(adj);
      enumNestedCalls(cg_, adj, visitedNodes_, target_);
    }
  }

  En(const CostGraph& cg, Hash& visitedNodes, OutIt target)
    : cg_(cg), visitedNodes_(visitedNodes), target_(target)
  {}

  ~En() override {}
};

template<typename Hash, typename OutIt> void enumNestedCalls(const CostGraph& cg,
                                                             CostGraph::NodeType* entry,
                                                             Hash& visitedNodes, OutIt target)
{
  En<Hash, OutIt> en{ cg, visitedNodes, target };
  entry->enumEdges(en);
}

}

CostGraph::NodeType* MmixLlvm::cg::findMostHeavyFunctionEntry(const CostGraph& cg)
{
  llvm::SmallVector< std::tuple<CostGraph::NodeType*, CostGraph::Cost>, 5 > nestedCalls;
  llvm::SmallPtrSet <CostGraph::NodeType*, 5> ctx;
  ctx.insert(cg.entry());
  enumNestedCalls(cg, cg.entry(), ctx, std::back_inserter(nestedCalls));
  auto maxIt = std::max_element(nestedCalls.begin(), nestedCalls.end(),
                                [](auto lho, auto rho) -> bool
  {
    return std::get<1>(lho) < std::get<1>(rho);
  });
  return std::get<0>(findMostHeavyCall0(cg, std::get<0>(*maxIt), std::get<1>(*maxIt)));
}