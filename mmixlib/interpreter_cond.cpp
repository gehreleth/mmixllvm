#include "stdafx.h"
#include "interpreter_common.h"
#include "interpreter_cond.h"

using namespace MmixLlvm;
using namespace MmixLlvm::InterpreterPrivate;

void MmixLlvm::InterpreterPrivate::scn(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::scni(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csz(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::cszi(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csp(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::cspi(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csnn(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::csnni(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csnz(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::csnzi(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csnp(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::csnpi(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csev(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::csevi(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::csod(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  }
}

void MmixLlvm::InterpreterPrivate::csodi(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  }
}

void MmixLlvm::InterpreterPrivate::zcn(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zcni(ExecutionContext& ctx, MXUTetra opc)
{
  if (ncond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsz(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zszi(ExecutionContext& ctx, MXUTetra opc)
{
  if (zcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsp(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zspi(ExecutionContext& ctx, MXUTetra opc)
{
  if (pcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnn(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnni(ExecutionContext& ctx, MXUTetra opc)
{
  if (nncond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnz(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnzi(ExecutionContext& ctx, MXUTetra opc)
{
  if (nzcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnp(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsnpi(ExecutionContext& ctx, MXUTetra opc)
{
  if (npcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsev(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsevi(ExecutionContext& ctx, MXUTetra opc)
{
  if (evcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsod(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, y(opc)))) {
    ctx.storeReg(x(opc), u(ctx, zu(opc)));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}

void MmixLlvm::InterpreterPrivate::zsodi(ExecutionContext& ctx, MXUTetra opc)
{
  if (odcond(s(ctx, y(opc)))) {
    ctx.storeReg<MXUOcta>(x(opc), zu(opc));
  } else {
    ctx.storeReg<MXUOcta>(x(opc), 0);
  }
}
