#ifndef ECONTEXT_H_DEFINED
#define ECONTEXT_H_DEFINED

#include "mmixdef.h"
#include "misc.h"
#include "stats_collector.h"
#include <type_traits>

namespace MmixLlvm {

class SyscallHandler;

namespace ExecutionContextPrivate {

template<typename Q> struct AbilityToStoreTest {
  enum {
    value =
      std::is_same<Q, MXUOcta>::value
    | std::is_same<Q, MXUTetra>::value
    | std::is_same<Q, MXUWyde>::value
    | std::is_same<Q, MXUByte>::value
    | std::is_same<Q, MXSOcta>::value
    | std::is_same<Q, MXSTetra>::value
    | std::is_same<Q, MXSWyde>::value
    | std::is_same<Q, MXSByte>::value
    | std::is_same<Q, MXFp>::value
    | std::is_same<Q, MXFps>::value
  };
};

template<typename Q> struct AbilityToStoreRegTest {
  enum {
    value =
      std::is_same<Q, MXUOcta>::value
    | std::is_same<Q, MXSOcta>::value
    | std::is_same<Q, MXFp>::value
  };
};

}

class ExecutionContext {
  void *const *const segments_;

  static_assert(sizeof(MXUOcta) == 8, "Assumption 1");
  static_assert(sizeof(MXUOcta) == sizeof(MXSOcta), "Assumption 2");
  static_assert(sizeof(MXUOcta) == sizeof(MXFp), "Assumption 3");

  union {
    MXUOcta uReg_[1 << LOCAL_REG_POW];
    MXSOcta sReg_[1 << LOCAL_REG_POW];
    MXFp fpReg_[1 << LOCAL_REG_POW];
  } rcache_;

  static_assert(sizeof(decltype(rcache_.uReg_)) == sizeof(decltype(rcache_.sReg_)), "Assumption 4");
  static_assert(sizeof(decltype(rcache_.uReg_)) == sizeof(decltype(rcache_.fpReg_)), "Assumption 5");

  union {
    MXUOcta uReg_[0x100];
    MXSOcta sReg_[0x100];
    MXFp fpReg_[0x100];
  } greg_;

  static_assert(sizeof(decltype(greg_.uReg_)) == sizeof(decltype(greg_.sReg_)), "Assumption 6");
  static_assert(sizeof(decltype(greg_.uReg_)) == sizeof(decltype(greg_.fpReg_)), "Assumption 7");

  template <typename R> struct TypeSwitch {
    static R* greg(ExecutionContext&);
    static R* locCache(ExecutionContext&);
  };

  template <typename R> friend struct TypeSwitch;

  MXUOcta spReg_[32];

  MXUOcta xptr_;

  MXUOcta xNextPtr_;

  MXUOcta ss_;

  MXUOcta stackBottom_;

  MXUOcta oo_;

  MXUOcta ll_;

  MXUOcta gg_;

  bool halted_;

  MXFp epsilon_;

  ArithFlag flags_;

  enum { ModRho = (1 << LOCAL_REG_POW) - 1 };

  size_t alpha();

  size_t beta();

  size_t gamma();

  template<typename T> T* mapAddress(MXUOcta base, MXUOcta offset)
  {
    enum : MXUOcta {
      Align = ~(sizeof(T) - 1),
      OffsetMask = (1ULL << 61) - 1,
#ifdef LITTLE_ENDIAN
      LeFix = sizeof(MXUOcta) - 1,
#else // Big Endian, apparently
      LeFix = 0,
#endif // LITTLE_ENDIAN
    };
    MXUOcta M = ((base + offset) ^ LeFix) & Align;
    size_t segNo = static_cast<size_t>((M >> 61) & 3);
    size_t segOffset = static_cast<size_t>(M & OffsetMask);
    void* ptr = static_cast<MXUByte*>(segments_[segNo]) + segOffset;
    return static_cast<T*>(ptr);
  }

  int deltaGrowBeta(int k);

  void growBeta(MXUByte size);

  void shrinkAlpha(MXUByte size);

  SyscallHandler* syscallHandler_;

  StatsCollector* statsCollector_;

  MXUOcta extractTrY(MXUTetra opc);

  MXUOcta extractTrZ(MXUTetra opc);
public:
  ExecutionContext(void** segments);

  ~ExecutionContext();

  void push(MXUByte x);

  void pop(MXUByte x);

  template<typename R> void storeReg(MXUByte reg, R val);

  template<typename R> R readReg(MXUByte reg);

  void storeSpReg(MmixLlvm::SpecialReg spReg, MXUOcta val);

  MXUOcta readSpReg(MmixLlvm::SpecialReg spReg);

  MXUTetra fetchInstruction()
  {
    return *mapAddress<MXUTetra>(0, (xptr_ = xNextPtr_++) << 2);
  }

  void setEntryPoint(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceInit(arg);
  }

  void setJumpAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceJump(getXptr(), arg);
  }

  void setTripAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceTrip(getXptr(), arg);
  }

  void setTrapAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceTrap(getXptr(), arg);
  }

  void setResumeFromTripAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceResumeFromTrip(getXptr(), arg);
  }

  void setResumeFromTrapAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceResumeFromTrap(getXptr(), arg);
  }

  void setJumpRel(MXSOcta arg)
  {
    MXUOcta dest = xptr_ + arg;
    xNextPtr_ = dest;
    statsCollector_->traceJump(getXptr(), dest << 2);
  }

  void setCondJumpRel(MXSOcta arg)
  {
    MXUOcta dest = xptr_ + arg;
    xNextPtr_ = dest;
    statsCollector_->traceCondJump(getXptr(), dest << 2);
  }

  void setCallRel(MXSOcta arg)
  {
    MXUOcta dest = xptr_ + arg;
    xNextPtr_ = dest;
    statsCollector_->traceCall(getXptr(), dest << 2);
  }

  void setCallAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->traceCall(getXptr(), arg);
  }

  void setPopAbs(MXUOcta arg)
  {
    xNextPtr_ = arg >> 2;
    statsCollector_->tracePop(getXptr(), arg);
  }

  MXUOcta getXptr()
  {
    return xptr_ << 2;
  }

  MXUOcta getXNextPtr()
  {
    return xNextPtr_ << 2;
  }

  MXFp epsilon()
  {
    return epsilon_;
  }

  ArithFlag& flags()
  {
    return flags_;
  }

  void registerSyscallHandler(SyscallHandler* handler);

  void registerStatsCollector(StatsCollector* collector);

  template<typename R> void storeMem(MXUOcta base, MXUOcta offset, R val)
  {
    static_assert(ExecutionContextPrivate::AbilityToStoreTest<R>::value, "Illegal type for storeMem");
    *mapAddress<R>(base, offset) = val;
  }

  template<typename R> R readMem(MXUOcta base, MXUOcta offset)
  {
    static_assert(ExecutionContextPrivate::AbilityToStoreTest<R>::value, "Illegal type for readMem");
    return *mapAddress<R>(base, offset);
  }

  void exportExternalBuffer(MXUOcta src, void* dest, size_t size);

  void importExternalBuffer(const void* src, MXUOcta dest, size_t size);

  void setCondition(MXUByte flag);

  void trip(MXUByte vector);

  void trap();

  void resumeFromTrip();

  void resumeFromTrap();

  void setHalt(bool val)
  {
    halted_ = val;
    if (halted_) {
      oo_ = 0;
    }
  }

  bool isHalted()
  {
    return halted_;
  }
};

template<> inline MXUOcta* ExecutionContext::TypeSwitch<MXUOcta>::locCache(ExecutionContext& ctx)
{
  return ctx.rcache_.uReg_;
}

template<> inline MXUOcta* ExecutionContext::TypeSwitch<MXUOcta>::greg(ExecutionContext& ctx)
{
  return ctx.greg_.uReg_;
}

template<> inline MXSOcta* ExecutionContext::TypeSwitch<MXSOcta>::locCache(ExecutionContext& ctx)
{
  return ctx.rcache_.sReg_;
}

template<> inline MXSOcta* ExecutionContext::TypeSwitch<MXSOcta>::greg(ExecutionContext& ctx)
{
  return ctx.greg_.sReg_;
}

template<> inline MXFp* ExecutionContext::TypeSwitch<MXFp>::locCache(ExecutionContext& ctx)
{
  return ctx.rcache_.fpReg_;
}

template<> inline MXFp* ExecutionContext::TypeSwitch<MXFp>::greg(ExecutionContext& ctx)
{
  return ctx.greg_.fpReg_;
}

template<typename R> inline void ExecutionContext::storeReg(MXUByte reg, R val)
{
  static_assert(ExecutionContextPrivate::AbilityToStoreRegTest<R>::value, "Invalid type for storeReg");
  if (reg < gg_) {
    int q = deltaGrowBeta(reg);
    if (q >= 0) {
      growBeta(q + 1);
    }
    ExecutionContext::TypeSwitch<R>::locCache(*this)[(alpha() + reg) & ModRho] = val;
  } else {
    ExecutionContext::TypeSwitch<R>::greg(*this)[0xff - reg] = val;
  }
}

template<typename R> inline R ExecutionContext::readReg(MXUByte reg)
{
  static_assert(ExecutionContextPrivate::AbilityToStoreRegTest<R>::value, "Invalid type for readReg");
  if (reg < gg_) {
    int q = deltaGrowBeta(reg);
    if (q >= 0) {
      growBeta(q + 1);
      return 0;
    } else {
      return ExecutionContext::TypeSwitch<R>::locCache(*this)[(alpha() + reg) & ModRho];
    }
  } else {
    return ExecutionContext::TypeSwitch<R>::greg(*this)[0xff - reg];
  }
}

}

#endif