#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>
#include <math.h>

using namespace MmixLlvm;

void testFpOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 5);
	ectx.storeReg<MXUOcta>(4, 7);
	ectx.storeReg<MXUOcta>(5, 13);
	ectx.storeReg<MXUOcta>(6, 17);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FLOT, 13, 0, 3));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, makeOpcode(MmixOpcode::FLOT, 14, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x18, makeOpcode(MmixOpcode::FLOT, 15, 0, 5));
	ectx.storeMem<MXUTetra>(0x6000, 0x1c, makeOpcode(MmixOpcode::FLOT, 16, 0, 6));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::FADD, 1, 13, 14));
	ectx.storeMem<MXUTetra>(0x6000, 0x24, makeOpcode(MmixOpcode::FDIV, 1, 1, 15));
	ectx.storeMem<MXUTetra>(0x6000, 0x28, makeOpcode(MmixOpcode::FMUL, 1, 1, 16));
	ectx.storeMem<MXUTetra>(0x6000, 0x2c, makeOpcode(MmixOpcode::FIX, 2, 2, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	MXFp q = ((5.0 + 7.0) / 13.0) * 17.0;
	assertEqualsF(q, ectx.readReg<MXFp>(1));
	assertEqualsU(16, ectx.readReg<MXUOcta>(2));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 5);
	ectx.storeReg<MXUOcta>(4, 7);
	ectx.storeReg<MXUOcta>(5, 2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FLOT, 2, 0, 5));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, makeOpcode(MmixOpcode::FSQRT, 1, 0, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x18, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsF(sqrt(2), ectx.readReg<MXFp>(1));

	ectx.storeReg<MXFp>(4, 3.1416);
	ectx.storeReg<MXUOcta>(5, -1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FLOT, 2, 0, 5));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, makeOpcode(MmixOpcode::FSQRT, 1, 0, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x18, makeOpcode(MmixOpcode::FUN, 2, 1, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x1c, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	union {
		MXFp r1;
		MXUOcta v1;
	};
	union {
		MXFp r2;
		MXUOcta v2;
	};
	r1 = sqrt(-1);
	r2 = ectx.readReg<MXFp>(1);
	assertEqualsF(v1, v2);
	assertEqualsU(I, ectx.readSpReg(SpecialReg::rA));
	assertEqualsU(1, ectx.readReg<MXUOcta>(2));

	ectx.storeReg<MXFp>(1, 5.3);
	ectx.storeReg<MXFp>(2, 2.0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FREM, 3, 1, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsF(std::remainder(5.3, 2), ectx.readReg<MXFp>(3));

	ectx.storeReg<MXFp>(1, 18.5);
	ectx.storeReg<MXFp>(2, 4.2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FREM, 3, 1, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsF(std::remainder(18.5, 4.2), ectx.readReg<MXFp>(3));

	ectx.storeReg<MXFp>(1, 4.2);
	ectx.storeReg<MXFp>(2, 4.2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FEQL, 3, 1, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1, ectx.readReg<MXSOcta>(3));

	ectx.storeReg<MXFp>(1, 4.2);
	ectx.storeReg<MXFp>(2, 4.3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FCMP, 3, 1, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXSOcta>(3));

	ectx.storeReg<MXFp>(1, 4.2);
	ectx.storeReg<MXFp>(2, 4.3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::FCMPE, 3, 1, 2));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXSOcta>(3));

	union {
		MXUTetra t;
		MXFps sf1;
	};

	sf1 = 3.1416f;
	MXFps sf2;
	
	ectx.storeMem(DATA_SEG, 0, t);
	ectx.storeReg<MXUOcta>(2, DATA_SEG);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDSFI, 1, 2, 0));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertTrue(fabs(ectx.readReg<MXFp>(1) - sf1) < 0.0000000001);

	ectx.storeReg<MXUOcta>(0, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDSF, 1, 2, 0));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertTrue(fabs(ectx.readReg<MXFp>(1) - sf1) < 0.0000000001);

	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STSFI, 1, 2, 16));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	sf2 = ectx.readMem<MXFps>(DATA_SEG, 16);

	ectx.storeReg<MXUOcta>(0, 16);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STSF, 1, 2, 0));
	ectx.storeMem<MXUTetra>(0x6000, 0x14, 0);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	sf2 = ectx.readMem<MXFps>(DATA_SEG, 16);

	assertTrue(sf1 == sf2);
}
