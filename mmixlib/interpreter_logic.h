#ifndef INTERPRETER_LOGIC_INCLUDED
#define INTERPRETER_LOGIC_INCLUDED

#include "mmixdef.h"
#include "econtext.h"
#include "interpreter_common.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void and_(ExecutionContext& ctx, MXUTetra opc);

void andi(ExecutionContext& ctx, MXUTetra opc);

void or_(ExecutionContext& ctx, MXUTetra opc);

void ori(ExecutionContext& ctx, MXUTetra opc);

void xor_(ExecutionContext& ctx, MXUTetra opc);

void xori(ExecutionContext& ctx, MXUTetra opc);

void andn(ExecutionContext& ctx, MXUTetra opc);

void andni(ExecutionContext& ctx, MXUTetra opc);

void orn(ExecutionContext& ctx, MXUTetra opc);

void orni(ExecutionContext& ctx, MXUTetra opc);

void nand(ExecutionContext& ctx, MXUTetra opc);

void nandi(ExecutionContext& ctx, MXUTetra opc);

void nor(ExecutionContext& ctx, MXUTetra opc);

void nori(ExecutionContext& ctx, MXUTetra opc);

void nxor(ExecutionContext& ctx, MXUTetra opc);

void nxori(ExecutionContext& ctx, MXUTetra opc);

void mux(ExecutionContext& ctx, MXUTetra opc);

void muxi(ExecutionContext& ctx, MXUTetra opc);

void sadd(ExecutionContext& ctx, MXUTetra opc);

void saddi(ExecutionContext& ctx, MXUTetra opc);

void orh(ExecutionContext& ctx, MXUTetra opc);

void ormh(ExecutionContext& ctx, MXUTetra opc);

void orml(ExecutionContext& ctx, MXUTetra opc);

void orl(ExecutionContext& ctx, MXUTetra opc);

void andnh(ExecutionContext& ctx, MXUTetra opc);

void andnmh(ExecutionContext& ctx, MXUTetra opc);

void andnml(ExecutionContext& ctx, MXUTetra opc);

void andnl(ExecutionContext& ctx, MXUTetra opc);

void bdif(ExecutionContext& ctx, MXUTetra opc);

void bdifi(ExecutionContext& ctx, MXUTetra opc);

void wdif(ExecutionContext& ctx, MXUTetra opc);

void wdifi(ExecutionContext& ctx, MXUTetra opc);

void tdif(ExecutionContext& ctx, MXUTetra opc);

void tdifi(ExecutionContext& ctx, MXUTetra opc);

void odif(ExecutionContext& ctx, MXUTetra opc);

void odifi(ExecutionContext& ctx, MXUTetra opc);

void mor(ExecutionContext& ctx, MXUTetra opc);

void mori(ExecutionContext& ctx, MXUTetra opc);

void mxor(ExecutionContext& ctx, MXUTetra opc);

void mxori(ExecutionContext& ctx, MXUTetra opc);

};

};

#endif