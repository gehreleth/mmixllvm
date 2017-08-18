#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

void stepInto(ExecutionContext& ctx, MXUOcta loc);

};

#endif