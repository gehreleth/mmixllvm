#ifndef INTERPRETER_LDST_INCLUDED
#define INTERPRETER_LDST_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void ldou(ExecutionContext& ctx, MXUTetra opc);

void ldtu(ExecutionContext& ctx, MXUTetra opc);

void ldwu(ExecutionContext& ctx, MXUTetra opc);

void ldbu(ExecutionContext& ctx, MXUTetra opc);

void ldoui(ExecutionContext& ctx, MXUTetra opc);

void ldtui(ExecutionContext& ctx, MXUTetra opc);

void ldwui(ExecutionContext& ctx, MXUTetra opc);

void ldbui(ExecutionContext& ctx, MXUTetra opc);

void ldo(ExecutionContext& ctx, MXUTetra opc);

void ldt(ExecutionContext& ctx, MXUTetra opc);

void ldw(ExecutionContext& ctx, MXUTetra opc);

void ldb(ExecutionContext& ctx, MXUTetra opc);

void ldoi(ExecutionContext& ctx, MXUTetra opc);

void ldti(ExecutionContext& ctx, MXUTetra opc);

void ldwi(ExecutionContext& ctx, MXUTetra opc);

void ldbi(ExecutionContext& ctx, MXUTetra opc);

void ldht(ExecutionContext& ctx, MXUTetra opc);

void ldhti(ExecutionContext& ctx, MXUTetra opc);

void stou(ExecutionContext& ctx, MXUTetra opc);

void sttu(ExecutionContext& ctx, MXUTetra opc);

void stwu(ExecutionContext& ctx, MXUTetra opc);

void stbu(ExecutionContext& ctx, MXUTetra opc);

void stoui(ExecutionContext& ctx, MXUTetra opc);

void sttui(ExecutionContext& ctx, MXUTetra opc);

void stwui(ExecutionContext& ctx, MXUTetra opc);

void stbui(ExecutionContext& ctx, MXUTetra opc);

void sto(ExecutionContext& ctx, MXUTetra opc);

void stt(ExecutionContext& ctx, MXUTetra opc);

void stw(ExecutionContext& ctx, MXUTetra opc);

void stb(ExecutionContext& ctx, MXUTetra opc);

void stoi(ExecutionContext& ctx, MXUTetra opc);

void stti(ExecutionContext& ctx, MXUTetra opc);

void stwi(ExecutionContext& ctx, MXUTetra opc);

void stbi(ExecutionContext& ctx, MXUTetra opc);

void stht(ExecutionContext& ctx, MXUTetra opc);

void sthti(ExecutionContext& ctx, MXUTetra opc);

void stco(ExecutionContext& ctx, MXUTetra opc);

void stcoi(ExecutionContext& ctx, MXUTetra opc);

void stsf(ExecutionContext& ctx, MXUTetra opc);

void stsfi(ExecutionContext& ctx, MXUTetra opc);

void ldsf(ExecutionContext& ctx, MXUTetra opc);

void ldsfi(ExecutionContext& ctx, MXUTetra opc);

void seth(ExecutionContext& ctx, MXUTetra opc);

void setmh(ExecutionContext& ctx, MXUTetra opc);

void setml(ExecutionContext& ctx, MXUTetra opc);

void setl(ExecutionContext& ctx, MXUTetra opc);

void get(ExecutionContext& ctx, MXUTetra opc);

void geta(ExecutionContext& ctx, MXUTetra opc);

void getab(ExecutionContext& ctx, MXUTetra opc);

void put(ExecutionContext& ctx, MXUTetra opc);

void puti(ExecutionContext& ctx, MXUTetra opc);
};
};

#endif
