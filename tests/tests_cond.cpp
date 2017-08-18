#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testCondOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSOD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSOD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSODI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSODI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSNPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSEV, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSEV, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSEVI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CSEVI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readReg<MXUOcta>(1));


	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSOD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSOD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSODI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSODI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNNI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNZ, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNZI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNP, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSNPI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSEV, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSEV, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSEVI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(7, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 13);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ZSEVI, 1, 2, 7));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));
}
