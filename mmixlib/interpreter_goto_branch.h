#ifndef INTERPRETER_GOTO_BRANCH_INCLUDED
#define INTERPRETER_GOTO_BRANCH_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

void jmp(ExecutionContext& ctx, MXUTetra opc);

void jmpb(ExecutionContext& ctx, MXUTetra opc);

void go(ExecutionContext& ctx, MXUTetra opc);

void goi(ExecutionContext& ctx, MXUTetra opc);

void bn(ExecutionContext& ctx, MXUTetra opc);

void bnb(ExecutionContext& ctx, MXUTetra opc);

void bz(ExecutionContext& ctx, MXUTetra opc);

void bzb(ExecutionContext& ctx, MXUTetra opc);

void bp(ExecutionContext& ctx, MXUTetra opc);

void bpb(ExecutionContext& ctx, MXUTetra opc);

void bod(ExecutionContext& ctx, MXUTetra opc);

void bodb(ExecutionContext& ctx, MXUTetra opc);

void bnn(ExecutionContext& ctx, MXUTetra opc);

void bnnb(ExecutionContext& ctx, MXUTetra opc);

void bnz(ExecutionContext& ctx, MXUTetra opc);

void bnzb(ExecutionContext& ctx, MXUTetra opc);

void bnp(ExecutionContext& ctx, MXUTetra opc);

void bnpb(ExecutionContext& ctx, MXUTetra opc);

void bev(ExecutionContext& ctx, MXUTetra opc);

void bevb(ExecutionContext& ctx, MXUTetra opc);

void pushj(ExecutionContext& ctx, MXUTetra opc);

void pushjb(ExecutionContext& ctx, MXUTetra opc);

void pushgo(ExecutionContext& ctx, MXUTetra opc);

void pushgoi(ExecutionContext& ctx, MXUTetra opc);

void pop(ExecutionContext& ctx, MXUTetra opc);

void trip(ExecutionContext& ctx, MXUTetra);

void trap(ExecutionContext& ctx, MXUTetra);

void resume(ExecutionContext& ctx, MXUTetra opc);

};

};


#endif