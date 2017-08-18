#include "stdafx.h"
#include "econtext.h"
#include "syscall_handler.h"
#include <intrin.h>

using namespace MmixLlvm;

ExecutionContext::ExecutionContext(void** segments)
  : segments_{ segments }
  , xptr_{ 0 }
  , halted_{ true }
  , ss_{ STACK_SEG >> 3 }
  , stackBottom_{ STACK_SEG >> 3 }
  , oo_{ 0 }
  , ll_{ 2 }
  , gg_{ 128 + 64 }
  , epsilon_{ DBL_EPSILON }
  , syscallHandler_{ nullptr }
{
  memset(rcache_.uReg_, 0, sizeof(rcache_.uReg_));
  memset(greg_.uReg_, 0, sizeof(greg_.uReg_));
  memset(spReg_, 0, sizeof(spReg_));
  registerStatsCollector(nullptr); // Empty handlers
}

ExecutionContext::~ExecutionContext() {}

MXUOcta ExecutionContext::extractTrY(MXUTetra opc)
{
  MXUByte Y = (opc >> 8) & 0xff;
  switch (static_cast<MmixOpcode>(opc >> 24)) {
    case MmixOpcode::TRIP:
    case MmixOpcode::TRAP:
      return Y;
    default:
      return readReg<MXUOcta>(Y);
  }
}

MXUOcta ExecutionContext::extractTrZ(MXUTetra opc)
{
  MXUByte Z = opc & 0xff;
  switch (static_cast<MmixOpcode>(opc >> 24)) {
    case MmixOpcode::TRIP:
    case MmixOpcode::TRAP:
      return Z;
    default:
      if (((opc >> 24) & 1) != 0) {
        return opc & 0xff;
      } else {
        return readReg<MXUOcta>(opc & 0xff);
      }
  }
}

inline int ExecutionContext::deltaGrowBeta(int k)
{
  return k - static_cast<int>(ll_);
}

void ExecutionContext::growBeta(MXUByte size)
{
  for (MXUByte i = 0; i < size; i++) {
    ++ll_;
    size_t gamma0 = gamma();
    if (gamma0 == beta() && gamma0 != alpha()) {
      storeMem(ss_ << 3, 0, rcache_.uReg_[gamma0]);
      ++ss_;
    }
  }
}

void ExecutionContext::shrinkAlpha(MXUByte size)
{
  for (MXUByte i = 0; i < size; i++) {
    --oo_;
    size_t gamma0 = gamma();
    if (gamma0 == alpha() && ss_ != stackBottom_ && gamma0 != beta()) {
      --ss_;
      rcache_.uReg_[gamma()] = readMem<MXUOcta>(ss_ << 3, 0);
    }
  }
}

void ExecutionContext::push(MXUByte x)
{
  // If X < rG, registers $0 to $X - 1 are pushed to the register stack.
  if (x < gg_) {
    storeReg<MXUOcta>(x, x); // $X <- regsPushed
    int regsPushed = x + 1; // [$0 .. $X]
    oo_ += regsPushed; // alpha <- $(X + 1)
    ll_ -= regsPushed; // beta <- $(X + L)
  } else {
    oo_ += ll_;
    ll_ = 0;
  }
}

void ExecutionContext::pop(MXUByte x)
{
  if (x < gg_) {
    MXUByte ll = static_cast<MXUByte>(ll_);
    MXUByte retain = x < ll ? x : ll;
    MXUOcta topVal = 0;
    if (retain != 0) {
      topVal = readReg<MXUOcta>(retain - 1);
    }
    MXUByte discard = ll - retain;
    shrinkAlpha(1);
    MXUByte regsPushed = static_cast<MXUByte>(rcache_.uReg_[alpha() & ModRho]);
    rcache_.uReg_[alpha() & ModRho] = topVal;
    shrinkAlpha(regsPushed);
    ll_ += (regsPushed - discard);
  } else {
    // TODO
  }
}

inline size_t ExecutionContext::alpha()
{
  return static_cast<size_t>(oo_ & ModRho);
}

inline size_t ExecutionContext::beta()
{
  return static_cast<size_t>((alpha() + ll_) & ModRho);
}

inline size_t ExecutionContext::gamma()
{
  return static_cast<size_t>(ss_ & ModRho);
}

void ExecutionContext::storeSpReg(MmixLlvm::SpecialReg spReg, MXUOcta val)
{
  switch (spReg) {
    case SpecialReg::rA:
      flags_.val = val;
    case SpecialReg::rO:
    case SpecialReg::rS:
    case SpecialReg::rL:
      break;
    case SpecialReg::rG:
      gg_ = val;
      break;
    case SpecialReg::rE: {
      union {
        MXFp q;
        MXUOcta w;
      };
      w = val;
      epsilon_ = q;
      break;
    }
    default:
      spReg_[static_cast<size_t>(spReg)] = val;
      break;
  }
}

MXUOcta ExecutionContext::readSpReg(MmixLlvm::SpecialReg spReg)
{
  switch (spReg) {
    case SpecialReg::rA:
      return flags_.val;
    case SpecialReg::rO:
      return oo_ << 3;
    case SpecialReg::rS:
      return ss_ << 3;
    case SpecialReg::rL:
      return ll_;
    case SpecialReg::rG:
      return gg_;
    case SpecialReg::rE: {
      union {
        MXFp q;
        MXUOcta w;
      };
      q = epsilon_;
      return w;
    }
    default:
      return spReg_[static_cast<size_t>(spReg)];
  }
}

void ExecutionContext::registerSyscallHandler(SyscallHandler* handler)
{
  syscallHandler_ = handler;
}

void ExecutionContext::registerStatsCollector(StatsCollector* collector)
{
  static class EmptyStatsCollector : public StatsCollector {
    void traceInit(MXUOcta to) override 
    { }
    void traceJump(MXUOcta from, MXUOcta to) override 
    { }
    void traceCondJump(MXUOcta from, MXUOcta to) override 
    { }
    void traceCall(MXUOcta from, MXUOcta to) override
    { }    
    void tracePop(MXUOcta from, MXUOcta to) override
    { }
    void traceTrip(MXUOcta from, MXUOcta to) override
    { }
    void traceTrap(MXUOcta from, MXUOcta to) override
    { }
    void traceResumeFromTrip(MXUOcta from, MXUOcta to) override
    { }
    void traceResumeFromTrap(MXUOcta from, MXUOcta to) override
    { }
  } emptyHandlers;
  if (collector) {
    statsCollector_ = collector;
  } else {
    statsCollector_ = &emptyHandlers;
  }
}

void ExecutionContext::exportExternalBuffer(MXUOcta src, void* dest, size_t size)
{
  size_t c = 0;
  uint8_t* p0 = static_cast<uint8_t*>(dest);
  while (c < size && (src + c) & 7) {
    *(p0 + c) = readMem<MXUByte>(src, c);
    ++c;
  }
  void* aligned = p0 + c;
  uint64_t* p1 = static_cast<uint64_t*>(aligned);
  size_t octasToCopy = (size - c) >> 3;
  for (size_t i = 0; i < octasToCopy; i++) {
    *(p1 + i) = _byteswap_uint64(readMem<MXUOcta>(src, c));
    c += 8;
  }
  while (c < size) {
    *(p0 + c) = readMem<MXUByte>(src, c);
    ++c;
  }
}

void ExecutionContext::importExternalBuffer(const void* src, MXUOcta dest, size_t size)
{
  size_t c = 0;
  const MXUByte* p0 = static_cast<const MXUByte*>(src);
  while (c < size && (dest + c) & 7) {
    storeMem<MXUByte>(dest, c, *(p0 + c));
    ++c;
  }
  const void* aligned = p0 + c;
  const MXUOcta* p1 = static_cast<const MXUOcta*>(aligned);
  size_t octasToCopy = (size - c) >> 3;
  for (size_t i = 0; i < octasToCopy; i++) {
    storeMem(dest, c, _byteswap_uint64(*(p1 + i))),
      c += 8;
  }
  while (c < size) {
    storeMem<MXUByte>(dest, c, *(p0 + c));
    ++c;
  }
}

namespace {
bool isStoreOpc(MXUTetra opc)
{
  switch (static_cast<MmixOpcode>(opc >> 24)) {
    case MmixOpcode::STOU:
    case MmixOpcode::STTU:
    case MmixOpcode::STWU:
    case MmixOpcode::STBU:
    case MmixOpcode::STOUI:
    case MmixOpcode::STTUI:
    case MmixOpcode::STWUI:
    case MmixOpcode::STBUI:
    case MmixOpcode::STO:
    case MmixOpcode::STT:
    case MmixOpcode::STW:
    case MmixOpcode::STB:
    case MmixOpcode::STOI:
    case MmixOpcode::STTI:
    case MmixOpcode::STWI:
    case MmixOpcode::STBI:
    case MmixOpcode::STHT:
    case MmixOpcode::STHTI:
    case MmixOpcode::STCO:
    case MmixOpcode::STCOI:
      return true;
    default:
      return false;
  }
}
}

void ExecutionContext::trip(MXUByte vector)
{
  storeSpReg(SpecialReg::rB, readReg<MXUOcta>(255));  // First, bB is set to $255
  storeReg(255, readSpReg(SpecialReg::rJ));    // Second, $255 is set to rJ
  storeSpReg(SpecialReg::rW, getXNextPtr()); // Third, rW is set to @ + 4
  MXUTetra tripOpc = readMem<MXUTetra>(0, getXptr());
  storeSpReg(SpecialReg::rX, (0x80000000ULL << 32) | tripOpc); // Left half of rX is set to 0x80000000,
                           // right - to the instruction that issued a trip
  if (!isStoreOpc(tripOpc)) {
    storeSpReg(SpecialReg::rY, extractTrY(tripOpc)); // If the opcode is not a store command, we save registers
    storeSpReg(SpecialReg::rZ, extractTrZ(tripOpc)); // or immediate constants
  } else {
    storeSpReg(SpecialReg::rY, extractTrY(tripOpc) + extractTrZ(tripOpc)); // Otherwise, rY <- M of the store command 
    storeSpReg(SpecialReg::rZ, readReg<MXUOcta>((tripOpc >> 16) & 0xff));           // Otherwise, rZ <- Value that can't be stored
  }
  setTripAbs(vector << 4);
}

void ExecutionContext::trap()
{
  storeSpReg(SpecialReg::rBB, readReg<MXUOcta>(255));  // First, bB is set to $255
  storeReg(255, readSpReg(SpecialReg::rJ));    // Second, $255 is set to rJ
  storeSpReg(SpecialReg::rWW, getXNextPtr()); // Third, rW is set to @ + 4
  MXUTetra trapOpc = readMem<MXUTetra>(0, getXptr());
  storeSpReg(SpecialReg::rXX, (0x80000000ULL << 32) | trapOpc); // Left half of rX is set to 0x80000000,
                            // right - to the instruction that issued a trip
  if (!isStoreOpc(trapOpc)) {
    storeSpReg(SpecialReg::rYY, extractTrY(trapOpc)); // If the opcode is not a store command, we save registers
    storeSpReg(SpecialReg::rZZ, extractTrZ(trapOpc)); // or immediate constants
  } else {
    storeSpReg(SpecialReg::rYY, extractTrY(trapOpc) + extractTrZ(trapOpc)); // Otherwise, rY <- M of the store command 
    storeSpReg(SpecialReg::rZZ, readReg<MXUOcta>((trapOpc >> 16) & 0xff));           // Otherwise, rZ <- Value that can't be stored
  }
  setTrapAbs(0);
  syscallHandler_->handleSyscall(*this, trapOpc);
}

void ExecutionContext::resumeFromTrip()
{
  setTripAbs(readSpReg(SpecialReg::rW));
  storeSpReg(SpecialReg::rJ, readReg<MXUOcta>(255));
  storeReg(255, readSpReg(SpecialReg::rB));
}

void ExecutionContext::resumeFromTrap()
{
  setResumeFromTrapAbs(readSpReg(SpecialReg::rWW));
  storeSpReg(SpecialReg::rJ, readReg<MXUOcta>(255));
  storeReg(255, readSpReg(SpecialReg::rBB));
}

void ExecutionContext::setCondition(MXUByte flag)
{
  if (!flags_.checkEvents(flag)) {
    flags_.val |= flag;
  } else {
    // Choose most significant vector. 
    // More significant vector should override the least one.
    MXUByte vector = 0;
    if ((flag & X) != 0 && flags_.bits.EX)
      vector = 8;
    if ((flag & Z) != 0 && flags_.bits.EZ)
      vector = 7;
    if ((flag & U) != 0 && flags_.bits.EU)
      vector = 6;
    if ((flag & O) != 0 && flags_.bits.EO)
      vector = 5;
    if ((flag & I) != 0 && flags_.bits.EI)
      vector = 4;
    if ((flag & W) != 0 && flags_.bits.EW)
      vector = 3;
    if ((flag & V) != 0 && flags_.bits.EV)
      vector = 2;
    if ((flag & D) != 0 && flags_.bits.ED)
      vector = 1;
    trip(vector);
  }
}

