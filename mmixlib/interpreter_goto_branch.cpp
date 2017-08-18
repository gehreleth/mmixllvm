#include "stdafx.h"
#include "mmixdef.h"
#include "econtext.h"
#include "interpreter_goto_branch.h"
#include "interpreter_common.h"

using namespace MmixLlvm;
using namespace MmixLlvm::InterpreterPrivate;

void MmixLlvm::InterpreterPrivate::jmp(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.setJumpRel(xyzof(opc));
}

void MmixLlvm::InterpreterPrivate::jmpb(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.setJumpRel(xyzob(opc));
}

void MmixLlvm::InterpreterPrivate::go(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta absPtr = u(ctx, y(opc)) + u(ctx, zu(opc));
  ctx.storeReg(x(opc), ctx.getXNextPtr());
  ctx.setJumpAbs(absPtr);
}

void MmixLlvm::InterpreterPrivate::goi(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta absPtr = u(ctx, y(opc)) + zu(opc);
  ctx.storeReg(x(opc), ctx.getXNextPtr());
  ctx.setJumpAbs(absPtr);
}

void MmixLlvm::InterpreterPrivate::bn(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnb(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bz(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bzb(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bp(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bpb(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bod(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bodb(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnn(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnnb(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnz(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnzb(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnp(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bnpb(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bev(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzof(opc));
  }
}

void MmixLlvm::InterpreterPrivate::bevb(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, x(opc)))) {
    ctx.setCondJumpRel(yzob(opc));
  }
}

void MmixLlvm::InterpreterPrivate::pushj(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeSpReg(SpecialReg::rJ, ctx.getXNextPtr());
  ctx.push(x(opc));
  ctx.setCallRel(yzof(opc));
}

void MmixLlvm::InterpreterPrivate::pushjb(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeSpReg(SpecialReg::rJ, ctx.getXNextPtr());
  ctx.push(x(opc));
  ctx.setCallRel(yzob(opc));
}

void MmixLlvm::InterpreterPrivate::pushgo(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta absPtr = u(ctx, y(opc)) + u(ctx, zu(opc));
  ctx.storeSpReg(SpecialReg::rJ, ctx.getXNextPtr());
  ctx.push(x(opc));
  ctx.setCallAbs(absPtr);
}

void MmixLlvm::InterpreterPrivate::pushgoi(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta absPtr = u(ctx, y(opc)) + zu(opc);
  ctx.storeSpReg(SpecialReg::rJ, ctx.getXNextPtr());
  ctx.push(x(opc));
  ctx.setCallAbs(absPtr);
}

void MmixLlvm::InterpreterPrivate::pop(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta absPtr = ctx.readSpReg(SpecialReg::rJ) + (yzu(opc) << 2);
  ctx.pop(x(opc));
  ctx.setPopAbs(absPtr);
}

void MmixLlvm::InterpreterPrivate::trip(ExecutionContext& ctx, MXUTetra)
{
  ctx.trip(0);
}

void MmixLlvm::InterpreterPrivate::trap(ExecutionContext& ctx, MXUTetra)
{
  ctx.trap();
}

void MmixLlvm::InterpreterPrivate::resume(ExecutionContext& ctx, MXUTetra opc)
{
  bool resumeFromTrip = zcond(zu(opc)) && nncond(static_cast<MXSOcta>(ctx.getXptr()));
  if (resumeFromTrip) {
    ctx.resumeFromTrip();
  } else {
    ctx.resumeFromTrap();
  }
}
