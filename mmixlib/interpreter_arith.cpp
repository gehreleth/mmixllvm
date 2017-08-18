#include "stdafx.h"
#include "interpreter_common.h"
#include "interpreter_arith.h"
#include "div128.h"
#include <intrin.h>

using namespace MmixLlvm;
using namespace MmixLlvm::InterpreterPrivate;

namespace {
inline MXSOcta checkedAdd(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  MXSOcta result = lho + rho;
  const MXSOcta octaSgnBit = std::numeric_limits<MXSOcta>::min();
  if (((lho ^ rho) & octaSgnBit) != ((lho ^ result) & octaSgnBit)) {
    ctx.setCondition(V);
  }
  return result;
}

inline MXSOcta checkedSub(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  MXSOcta result = lho - rho;
  const MXSOcta octaSgnBit = std::numeric_limits<MXSOcta>::min();
  if ((rho == octaSgnBit) || (((lho ^ -rho) & octaSgnBit) != ((lho ^ result) & octaSgnBit))) {
    ctx.setCondition(V);
  }
  return result;
}

inline MXSOcta checkedDiv(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  if (rho != 0) {
    auto result = std::lldiv(lho, rho);
    ctx.storeSpReg(SpecialReg::rR, result.rem);
    return result.quot;
  } else {
    ctx.storeSpReg(SpecialReg::rR, lho);
    ctx.setCondition(D);
    return 0;
  }
}

inline MXUOcta uncheckedUDiv(ExecutionContext& ctx, MXUOcta lho, MXUOcta rho)
{
  if (rho != 0 && rho > ctx.readSpReg(SpecialReg::rD)) {
    MXUOcta rem;
    MXUOcta result = udiv128(ctx.readSpReg(SpecialReg::rD), lho, rho, &rem);
    ctx.storeSpReg(SpecialReg::rR, rem);
    return result;
  } else {
    ctx.storeSpReg(SpecialReg::rR, lho);
    return ctx.readSpReg(SpecialReg::rD);
  }
}


inline MXSOcta checkedMul(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  MXSOcta ho;
  MXSOcta result = _mul128(lho, rho, &ho);
  if (!((result >= 0 && ho == 0) || (result < 0 && ho == -1))) {
    ctx.setCondition(V);
  }
  return result;
}

inline MXUOcta uncheckedUMul(ExecutionContext& ctx, MXUOcta lho, MXUOcta rho)
{
  MXUOcta humulut;
  MXUOcta result = _umul128(lho, rho, &humulut);
  ctx.storeSpReg(SpecialReg::rH, humulut);
  return result;
}

inline MXUOcta checkedSl(ExecutionContext& ctx, MXSOcta lho, MXUByte rho)
{
  const MXSOcta hp = lho >= 0 ? 0 : -1;
  MXSOcta result = lho << rho;
  bool lostBits = hp != __shiftleft128(static_cast<MXUOcta>(lho), static_cast<MXUOcta>(hp), rho);
  bool lostSign = ((result ^ lho) & std::numeric_limits<MXSOcta>::min()) != 0;
  if (lostSign || lostBits) {
    ctx.setCondition(V);
  }
  return result;
}
};

void MmixLlvm::InterpreterPrivate::add(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedAdd(ctx, s(ctx, y(opc)), s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::addi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedAdd(ctx, s(ctx, y(opc)), zs(opc)));
}

void MmixLlvm::InterpreterPrivate::sub(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSub(ctx, s(ctx, y(opc)), s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::subi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSub(ctx, s(ctx, y(opc)), zs(opc)));
}

void MmixLlvm::InterpreterPrivate::addu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) + u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::addui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) + zu(opc));
}

void MmixLlvm::InterpreterPrivate::inch(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, x(opc)) + (yzu(opc) << 48));
}

void MmixLlvm::InterpreterPrivate::incmh(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, x(opc)) + (yzu(opc) << 32));
}

void MmixLlvm::InterpreterPrivate::incml(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, x(opc)) + (yzu(opc) << 16));
}

void MmixLlvm::InterpreterPrivate::incl(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, x(opc)) + yzu(opc));
}

void MmixLlvm::InterpreterPrivate::subu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) - u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::subui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) - zu(opc));
}

void MmixLlvm::InterpreterPrivate::neg(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSub(ctx, static_cast<MXSByte>(y(opc)), s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::negi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSub(ctx, static_cast<MXSByte>(y(opc)), zs(opc)));
}

void MmixLlvm::InterpreterPrivate::negu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), y(opc) - u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::negui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), y(opc) - zu(opc));
}

void MmixLlvm::InterpreterPrivate::_2addu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 1) + u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::_2addui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 1) + zu(opc));
}

void MmixLlvm::InterpreterPrivate::_4addu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 2) + u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::_4addui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 2) + zu(opc));
}

void MmixLlvm::InterpreterPrivate::_8addu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 3) + u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::_8addui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 3) + zu(opc));
}

void MmixLlvm::InterpreterPrivate::_16addu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 4) + u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::_16addui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), (u(ctx, y(opc)) << 4) + zu(opc));
}

void MmixLlvm::InterpreterPrivate::div(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedDiv(ctx, s(ctx, y(opc)), s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::divi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedDiv(ctx, s(ctx, y(opc)), zs(opc)));
}

void MmixLlvm::InterpreterPrivate::divu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), uncheckedUDiv(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::divui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), uncheckedUDiv(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::mul(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedMul(ctx, s(ctx, y(opc)), s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::muli(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedMul(ctx, s(ctx, y(opc)), zs(opc)));
}

void MmixLlvm::InterpreterPrivate::mulu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), uncheckedUMul(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::mului(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), uncheckedUMul(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::sl(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSl(ctx, s(ctx, y(opc)), static_cast<MXUByte>(u(ctx, zu(opc)))));
}

void MmixLlvm::InterpreterPrivate::sli(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), checkedSl(ctx, s(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::sr(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), s(ctx, y(opc)) >> u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::sri(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), s(ctx, y(opc)) >> zu(opc));
}

void MmixLlvm::InterpreterPrivate::slu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) << u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::slui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) << zu(opc));
}

void MmixLlvm::InterpreterPrivate::sru(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) >> u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::srui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) >> zu(opc));
}

void MmixLlvm::InterpreterPrivate::cmp(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXSOcta>(s(ctx, y(opc)) > s(ctx, zu(opc))) - static_cast<MXSOcta>(s(ctx, y(opc)) < s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::cmpi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXSOcta>(s(ctx, y(opc)) > zu(opc)) - static_cast<MXSOcta>(s(ctx, y(opc)) < zu(opc)));
}

void MmixLlvm::InterpreterPrivate::cmpu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXSOcta>(u(ctx, y(opc)) > u(ctx, zu(opc))) - static_cast<MXSOcta>(u(ctx, y(opc)) < u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::cmpui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXSOcta>(u(ctx, y(opc)) > zu(opc)) - static_cast<MXSOcta>(u(ctx, y(opc)) < zu(opc)));
}
