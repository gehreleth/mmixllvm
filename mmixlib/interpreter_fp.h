#ifndef INTERPRETER_FP_INCLUDED
#define INTERPRETER_FP_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void fix(ExecutionContext& ctx, MXUTetra opc);
void fixu(ExecutionContext& ctx, MXUTetra opc);
void flot(ExecutionContext& ctx, MXUTetra opc);
void floti(ExecutionContext& ctx, MXUTetra opc);
void flotu(ExecutionContext& ctx, MXUTetra opc);
void flotui(ExecutionContext& ctx, MXUTetra opc);
void fadd(ExecutionContext& ctx, MXUTetra opc);
void fsub(ExecutionContext& ctx, MXUTetra opc);
void fmul(ExecutionContext& ctx, MXUTetra opc);
void fdiv(ExecutionContext& ctx, MXUTetra opc);
void frem(ExecutionContext& ctx, MXUTetra opc);
void fsqrt(ExecutionContext& ctx, MXUTetra opc);
void fint(ExecutionContext& ctx, MXUTetra opc);
void fcmp(ExecutionContext& ctx, MXUTetra opc);
void feql(ExecutionContext& ctx, MXUTetra opc);
void fun(ExecutionContext& ctx, MXUTetra opc);
void fcmpe(ExecutionContext& ctx, MXUTetra opc);
void feqle(ExecutionContext& ctx, MXUTetra opc);
void fune(ExecutionContext& ctx, MXUTetra opc);
void sflot(ExecutionContext& ctx, MXUTetra opc);
void sfloti(ExecutionContext& ctx, MXUTetra opc);
void sflotu(ExecutionContext& ctx, MXUTetra opc);
void sflotui(ExecutionContext& ctx, MXUTetra opc);
void sflotui(ExecutionContext& ctx, MXUTetra opc);

}

}

#endif