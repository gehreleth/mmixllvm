// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define NOMINMAX

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <econtext.h>
#include <vamemstretegy.h>
#include <mmo_reader_dest_ref.h>
#include <mmixdef.h>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <vector>
#include <stack>
#include <iterator>
#include <tuple>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SetVector.h>
#define _USE_MATH_DEFINES
#include <cmath>

namespace MmixLlvm {
	typedef ExecutionContext ECtx;
}

typedef MmixLlvm::MmoLoaderDestRef DR;

extern void testJit(MmixLlvm::ECtx& ectx);

extern void assertTrue(bool arg, const char* errMessage = "Error !");

extern void assertEqualsU(MmixLlvm::MXUOcta expected, MmixLlvm::MXUOcta actual, const char* errMessage = "Error !");

extern void assertEqualsS(MmixLlvm::MXSOcta expected, MmixLlvm::MXSOcta actual, const char* errMessage = "Error !");

extern void assertEqualsF(MmixLlvm::MXFp expected, MmixLlvm::MXFp actual, const char* errMessage = "Error !");

extern MmixLlvm::MXUTetra makeOpcode(MmixLlvm::MmixOpcode opc, MmixLlvm::MXUByte x, MmixLlvm::MXUByte y, MmixLlvm::MXUByte z);

extern void storeString(MmixLlvm::ECtx& ectx, MmixLlvm::MXUOcta base, MmixLlvm::MXUOcta offset, const char* arg);

extern std::string readString(MmixLlvm::ECtx& ectx, MmixLlvm::MXUOcta base, MmixLlvm::MXUOcta offset);

extern void disableFlags(MmixLlvm::ECtx& ctx);

extern void testArithFlag(MmixLlvm::ECtx& ectx);

extern void testStoreAndLoad(MmixLlvm::ECtx& ctx);

extern void testStack(MmixLlvm::ECtx& ctx);

extern void testLoadMmo(MmixLlvm::ECtx& ectx);

extern void testLoadOpcopes(MmixLlvm::ECtx& ectx);

extern void testStoreOpcopes(MmixLlvm::ECtx& ectx);

extern void testArithOpcopes(MmixLlvm::ECtx& ectx);

extern void testCondOpcopes(MmixLlvm::ECtx& ectx);

extern void testLogicOpcopes(MmixLlvm::ECtx& ectx);

extern void testGotoOpcopes(MmixLlvm::ECtx& ectx);

extern void testFpOpcopes(MmixLlvm::ECtx& ectx);

extern void testRudimentaryIo(MmixLlvm::ECtx& ectx);

extern void testCostGraph(MmixLlvm::ECtx& ectx);

// TODO: reference additional headers your program requires here
