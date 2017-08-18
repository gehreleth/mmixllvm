#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testArithFlag(MmixLlvm::ECtx& ectx) {
	ArithFlag af;
	size_t l1 = sizeof(ArithFlag);
	size_t l2 = sizeof(MXUOcta);
	assertTrue(l1 == l2);
	af.bits.FX = true;
	assertTrue(af.val == X);
	af.val = 0;
	af.bits.EX = true;
	assertTrue(af.val == (X << 8));
	af.val = 0;
	af.bits.roundMode = 1;
	assertTrue(af.val == (1 << 16));
	af.val = 0;
	af.bits.EX = true;
	af.bits.ED = true;
	assertTrue(af.checkEvents(X | D));
}