#ifndef INTERPRETER_ARITH_INCLUDED
#define INTERPRETER_ARITH_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void add(ExecutionContext& ctx, MXUTetra opc);

void addi(ExecutionContext& ctx, MXUTetra opc);

void sub(ExecutionContext& ctx, MXUTetra opc);

void subi(ExecutionContext& ctx, MXUTetra opc);

void addu(ExecutionContext& ctx, MXUTetra opc);

void addui(ExecutionContext& ctx, MXUTetra opc);

void inch(ExecutionContext& ctx, MXUTetra opc);

void incmh(ExecutionContext& ctx, MXUTetra opc);

void incml(ExecutionContext& ctx, MXUTetra opc);

void incl(ExecutionContext& ctx, MXUTetra opc);

void subu(ExecutionContext& ctx, MXUTetra opc);

void subui(ExecutionContext& ctx, MXUTetra opc);

void neg(ExecutionContext& ctx, MXUTetra opc);

void negi(ExecutionContext& ctx, MXUTetra opc);

void negu(ExecutionContext& ctx, MXUTetra opc);

void negui(ExecutionContext& ctx, MXUTetra opc);

void _2addu(ExecutionContext& ctx, MXUTetra opc);

void _2addui(ExecutionContext& ctx, MXUTetra opc);

void _4addu(ExecutionContext& ctx, MXUTetra opc);

void _4addui(ExecutionContext& ctx, MXUTetra opc);

void _8addu(ExecutionContext& ctx, MXUTetra opc);

void _8addui(ExecutionContext& ctx, MXUTetra opc);

void _16addu(ExecutionContext& ctx, MXUTetra opc);

void _16addui(ExecutionContext& ctx, MXUTetra opc);

void div(ExecutionContext& ctx, MXUTetra opc);

void divi(ExecutionContext& ctx, MXUTetra opc);

void divu(ExecutionContext& ctx, MXUTetra opc);

void divui(ExecutionContext& ctx, MXUTetra opc);

void mul(ExecutionContext& ctx, MXUTetra opc);

void muli(ExecutionContext& ctx, MXUTetra opc);

void mulu(ExecutionContext& ctx, MXUTetra opc);

void mului(ExecutionContext& ctx, MXUTetra opc);

void sl(ExecutionContext& ctx, MXUTetra opc);

void sli(ExecutionContext& ctx, MXUTetra opc);

void sr(ExecutionContext& ctx, MXUTetra opc);

void sri(ExecutionContext& ctx, MXUTetra opc);

void slu(ExecutionContext& ctx, MXUTetra opc);

void slui(ExecutionContext& ctx, MXUTetra opc);

void sru(ExecutionContext& ctx, MXUTetra opc);

void srui(ExecutionContext& ctx, MXUTetra opc);

void cmp(ExecutionContext& ctx, MXUTetra opc);

void cmpi(ExecutionContext& ctx, MXUTetra opc);

void cmpu(ExecutionContext& ctx, MXUTetra opc);

void cmpui(ExecutionContext& ctx, MXUTetra opc);

};

};

#endif
