#ifndef INTERPRETER_COND_INCLUDED
#define INTERPRETER_COND_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void scn(ExecutionContext& ctx, MXUTetra opc);

void scni(ExecutionContext& ctx, MXUTetra opc);

void csz(ExecutionContext& ctx, MXUTetra opc);

void cszi(ExecutionContext& ctx, MXUTetra opc);

void csp(ExecutionContext& ctx, MXUTetra opc);

void cspi(ExecutionContext& ctx, MXUTetra opc);

void csnn(ExecutionContext& ctx, MXUTetra opc);

void csnni(ExecutionContext& ctx, MXUTetra opc);

void csnz(ExecutionContext& ctx, MXUTetra opc);

void csnzi(ExecutionContext& ctx, MXUTetra opc);

void csnp(ExecutionContext& ctx, MXUTetra opc);

void csnpi(ExecutionContext& ctx, MXUTetra opc);

void csev(ExecutionContext& ctx, MXUTetra opc);

void csevi(ExecutionContext& ctx, MXUTetra opc);

void csod(ExecutionContext& ctx, MXUTetra opc);

void csodi(ExecutionContext& ctx, MXUTetra opc);

void zcn(ExecutionContext& ctx, MXUTetra opc);

void zcni(ExecutionContext& ctx, MXUTetra opc);

void zsz(ExecutionContext& ctx, MXUTetra opc);

void zszi(ExecutionContext& ctx, MXUTetra opc);

void zsp(ExecutionContext& ctx, MXUTetra opc);

void zspi(ExecutionContext& ctx, MXUTetra opc);

void zsnn(ExecutionContext& ctx, MXUTetra opc);

void zsnni(ExecutionContext& ctx, MXUTetra opc);

void zsnz(ExecutionContext& ctx, MXUTetra opc);

void zsnzi(ExecutionContext& ctx, MXUTetra opc);

void zsnp(ExecutionContext& ctx, MXUTetra opc);

void zsnpi(ExecutionContext& ctx, MXUTetra opc);

void zsev(ExecutionContext& ctx, MXUTetra opc);

void zsevi(ExecutionContext& ctx, MXUTetra opc);

void zsod(ExecutionContext& ctx, MXUTetra opc);

void zsodi(ExecutionContext& ctx, MXUTetra opc);

};

};

#endif
