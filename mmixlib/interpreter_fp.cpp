#include "stdafx.h"
#include "interpreter_common.h"
#include "interpreter_fp.h"

using namespace MmixLlvm;

namespace {

MXFp toFint(MXFp fp, RoundMode mode, MXUByte& flags)
{
  MXFp retVal = 0.;
  std::feclearexcept(FE_ALL_EXCEPT);
  auto crtOrigRound = std::fegetround();
  switch (mode) {
    case MmixLlvm::ROUND_OFF:
      std::fesetround(FE_TOWARDZERO);
      retVal = std::rint(fp);
      break;
    case MmixLlvm::ROUND_UP:
      std::fesetround(FE_UPWARD);
      retVal = std::rint(fp);
      break;
    case MmixLlvm::ROUND_DOWN:
      std::fesetround(FE_DOWNWARD);
      retVal = std::rint(fp);
      break;
    case MmixLlvm::ROUND_NEAR:
      std::fesetround(FE_TONEAREST);
      retVal = std::rint(fp);
      break;
    default:
      break;
  }
  std::fesetround(crtOrigRound);
  if (std::fetestexcept(FE_INEXACT)) {
    flags |= X;
  }
  if (retVal >= std::numeric_limits<MXSOcta>::max() || retVal < std::numeric_limits<MXSOcta>::min()) {
    flags |= W;
  }
  return retVal;
}

MXUByte translateExceptions(int exc)
{
  MXUByte retVal = 0;
  if ((exc & FE_INEXACT) != 0) {
    retVal |= X;
  }
  if ((exc & FE_UNDERFLOW) != 0) {
    retVal |= U;
  }
  if ((exc & FE_OVERFLOW) != 0) {
    retVal |= O;
  }
  if ((exc & FE_DIVBYZERO) != 0) {
    retVal |= Z;
  }
  if ((exc & FE_INVALID) != 0) {
    retVal |= I;
  }
  return retVal;
}

}

void MmixLlvm::InterpreterPrivate::fix(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp fp = f(ctx, zu(opc));
  RoundMode mode = roundMode(ctx, y(opc));
  MXUByte flags = 0;
  ctx.storeReg<MXSOcta>(x(opc), static_cast<MXSOcta>(toFint(fp, mode, flags)));
  if (flags != 0) {
    ctx.setCondition(flags);
  }
}

void MmixLlvm::InterpreterPrivate::fixu(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp fp = f(ctx, zu(opc));
  RoundMode mode = roundMode(ctx, y(opc));
  MXUByte flags = 0;
  ctx.storeReg<MXUOcta>(x(opc), static_cast<MXUOcta>(toFint(fp, mode, flags)));
  if (flags != 0) {
    ctx.setCondition(flags);
  }
}

void MmixLlvm::InterpreterPrivate::fint(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp fp = f(ctx, zu(opc));
  RoundMode mode = roundMode(ctx, y(opc));
  MXUByte flags = 0;
  ctx.storeReg<MXFp>(x(opc), toFint(fp, mode, flags));
  if (flags != 0) {
    ctx.setCondition(flags);
  }
}

void MmixLlvm::InterpreterPrivate::flot(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXFp>(s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::floti(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXFp>(zs(opc)));
}

void MmixLlvm::InterpreterPrivate::flotu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXFp>(u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::flotui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), static_cast<MXFp>(zu(opc)));
}

void MmixLlvm::InterpreterPrivate::sflot(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXFp>(x(opc), static_cast<MXFps>(s(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::sfloti(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXFp>(x(opc), static_cast<MXFps>(zs(opc)));
}

void MmixLlvm::InterpreterPrivate::sflotu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXFp>(x(opc), static_cast<MXFps>(u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::sflotui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXFp>(x(opc), static_cast<MXFps>(zu(opc)));
}

void MmixLlvm::InterpreterPrivate::fadd(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = f(ctx, y(opc)) + f(ctx, zu(opc));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fsub(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = f(ctx, y(opc)) - f(ctx, zu(opc));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fmul(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = f(ctx, y(opc)) * f(ctx, zu(opc));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fdiv(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = f(ctx, y(opc)) / f(ctx, zu(opc));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::frem(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = std::remainder(f(ctx, y(opc)), f(ctx, zu(opc)));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fsqrt(ExecutionContext& ctx, MXUTetra opc)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  MXFp r = std::sqrt(f(ctx, zu(opc)));
  int exc = std::fetestexcept(FE_ALL_EXCEPT);
  if (exc != 0) {
    ctx.setCondition(translateExceptions(exc));
  }
  ctx.storeReg<MXFp>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fcmp(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp fy = f(ctx, y(opc));
  MXFp fz = f(ctx, zu(opc));
  MXSOcta r = 0;
  if (!(std::isnan(fy) || std::isnan(fy))) {
    r = (fy > fz) - (fy < fz);
  } else {
    ctx.setCondition(I);
  }
  ctx.storeReg<MXSOcta>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::feql(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp fy = f(ctx, y(opc));
  MXFp fz = f(ctx, zu(opc));
  MXSOcta r = 0;
  if (!(std::isnan(fy) || std::isnan(fy))) {
    r = (fy == fz);
  }
  ctx.storeReg<MXSOcta>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fun(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta r = std::isunordered(f(ctx, y(opc)), f(ctx, zu(opc)));
  ctx.storeReg<MXSOcta>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fcmpe(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta r = 0;
  MXFp fy = f(ctx, y(opc));
  MXFp fz = f(ctx, zu(opc));
  MXFp epsilon = ctx.epsilon();
  if (!(std::isnan(fy) || std::isnan(fz) || std::isnan(epsilon) || std::signbit(epsilon))) {
    int exponent;
    MXFp delta;
    MXFp difference;
    std::frexp(std::fabs(fy) > std::fabs(fz) ? fy : fz, &exponent);
    delta = std::ldexp(epsilon, exponent);
    difference = fy - fz;
    if (difference > delta) {
      r = 1;
    } else if (difference < -delta) {
      r = -1;
    }
  } else {
    ctx.setCondition(I);
  }
  ctx.storeReg<MXSOcta>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::feqle(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta r = 0;
  MXFp fy = f(ctx, y(opc));
  MXFp fz = f(ctx, zu(opc));
  MXFp epsilon = ctx.epsilon();
  if (!(std::isnan(fy) || std::isnan(fz) || std::isnan(epsilon) || std::signbit(epsilon))) {
    int exponent;
    MXFp delta;
    MXFp difference;
    std::frexp(std::fabs(fy) > std::fabs(fz) ? fy : fz, &exponent);
    delta = std::ldexp(epsilon, exponent);
    difference = fy - fz;
    if (difference >= -delta && difference < delta) {
      r = 1;
    }
  } else {
    ctx.setCondition(I);
  }
  ctx.storeReg<MXSOcta>(x(opc), r);
}

void MmixLlvm::InterpreterPrivate::fune(ExecutionContext& ctx, MXUTetra opc)
{
  MXFp epsilon = ctx.epsilon();
  MXSOcta r = std::isunordered(f(ctx, y(opc)), f(ctx, zu(opc))) || std::isnan(epsilon) || std::signbit(epsilon);
  ctx.storeReg<MXSOcta>(x(opc), r);
}
