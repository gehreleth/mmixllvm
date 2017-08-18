#include "stdafx.h"
#include "mmixdef.h"
#include "econtext.h"
#include "interpreter_common.h"
#include "interpreter_ldst.h"
#include <limits>

using namespace MmixLlvm;
using namespace MmixLlvm::InterpreterPrivate;

void MmixLlvm::InterpreterPrivate::ldou(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUOcta>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldtu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUTetra>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldwu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUWyde>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldbu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUByte>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldoui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUOcta>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldtui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUTetra>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldwui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUWyde>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldbui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), ctx.readMem<MXUByte>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldo(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSOcta>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldt(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSTetra>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldw(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSWyde>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldb(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSByte>(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::ldoi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSOcta>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldti(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSTetra>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldwi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSWyde>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldbi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXSOcta>(x(opc), ctx.readMem<MXSByte>(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ldht(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), static_cast<MXUOcta>(ctx.readMem<MXUTetra>(u(ctx, y(opc)), u(ctx, zu(opc)))) << 32);
}

void MmixLlvm::InterpreterPrivate::ldhti(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg<MXUOcta>(x(opc), static_cast<MXUOcta>(ctx.readMem<MXUTetra>(u(ctx, y(opc)), zu(opc))) << 32);
}

void MmixLlvm::InterpreterPrivate::stou(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), u(ctx, x(opc)));
}

void MmixLlvm::InterpreterPrivate::sttu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXUTetra>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stwu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXUWyde>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stbu(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXUByte>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stoui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), u(ctx, x(opc)));
}

void MmixLlvm::InterpreterPrivate::sttui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXUTetra>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stwui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXUWyde>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stbui(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXUByte>(u(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::sto(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), u(ctx, x(opc)));
}

void MmixLlvm::InterpreterPrivate::stt(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXSTetra>(o));
  if (o < std::numeric_limits<MXSTetra>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stw(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXSWyde>(o));
  if (o < std::numeric_limits<MXSWyde>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stb(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXSByte>(o));
  if (o < std::numeric_limits<MXSByte>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stoi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), s(ctx, x(opc)));
}

void MmixLlvm::InterpreterPrivate::stti(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXSTetra>(o));
  if (o < std::numeric_limits<MXSTetra>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stwi(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXSWyde>(o));
  if (o < std::numeric_limits<MXSWyde>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stbi(ExecutionContext& ctx, MXUTetra opc)
{
  MXSOcta o = s(ctx, x(opc));
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXSByte>(o));
  if (o < std::numeric_limits<MXSByte>::min()) {
    ctx.setCondition(V);
  }
}

void MmixLlvm::InterpreterPrivate::stht(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXUTetra>(u(ctx, x(opc)) >> 32));
}

void MmixLlvm::InterpreterPrivate::sthti(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXUTetra>(u(ctx, x(opc)) >> 32));
}

void MmixLlvm::InterpreterPrivate::stco(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem<MXUOcta>(u(ctx, y(opc)), u(ctx, zu(opc)), x(opc));
}

void MmixLlvm::InterpreterPrivate::stcoi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem<MXUOcta>(u(ctx, y(opc)), zu(opc), x(opc));
}

void MmixLlvm::InterpreterPrivate::stsf(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), u(ctx, zu(opc)), static_cast<MXFps>(f(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::stsfi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeMem(u(ctx, y(opc)), zu(opc), static_cast<MXFps>(f(ctx, x(opc))));
}

void MmixLlvm::InterpreterPrivate::ldsf(ExecutionContext& ctx, MXUTetra opc)
{
  MXFps val = ctx.readMem<MXFps>(u(ctx, y(opc)), u(ctx, zu(opc)));
  ctx.storeReg<MXFp>(x(opc), val);
}

void MmixLlvm::InterpreterPrivate::ldsfi(ExecutionContext& ctx, MXUTetra opc)
{
  MXFps val = ctx.readMem<MXFps>(u(ctx, y(opc)), zu(opc));
  ctx.storeReg<MXFp>(x(opc), val);
}

void MmixLlvm::InterpreterPrivate::seth(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), yzu(opc) << 48);
}

void MmixLlvm::InterpreterPrivate::setmh(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), yzu(opc) << 32);
}

void MmixLlvm::InterpreterPrivate::setml(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), yzu(opc) << 16);
}

void MmixLlvm::InterpreterPrivate::setl(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), yzu(opc));
}

void MmixLlvm::InterpreterPrivate::get(ExecutionContext& ctx, MXUTetra opc)
{
  MXUByte reg = zu(opc);
  if (reg < 32) {
    ctx.storeReg(x(opc), ctx.readSpReg(static_cast<SpecialReg>(reg)));
  }
}

void MmixLlvm::InterpreterPrivate::geta(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta at = ctx.getXptr();
  MXSOcta offset = yzs(opc) << 2;
  ctx.storeReg(x(opc), at + offset);
}

void MmixLlvm::InterpreterPrivate::getab(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta at = ctx.getXptr();
  MXSOcta offset = yzs(opc) << 2;
  ctx.storeReg(x(opc), at + offset);
}

namespace {
void putImpl(ExecutionContext& ctx, MXUByte reg, MXUOcta val)
{
  if (reg < 32 && !(reg >= 8 && reg <= 18)) {
    ctx.storeSpReg(static_cast<SpecialReg>(reg), val);
  }
}
};

void MmixLlvm::InterpreterPrivate::put(ExecutionContext& ctx, MXUTetra opc)
{
  putImpl(ctx, x(opc), u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::puti(ExecutionContext& ctx, MXUTetra opc)
{
  putImpl(ctx, x(opc), zu(opc));
}