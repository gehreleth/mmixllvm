#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testGotoOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::JMP, 0, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));


	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::JMPB, 0xff, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x1000);
	ectx.storeReg<MXUOcta>(3, 0x10);
	ectx.storeReg<MXUOcta>(4, 0x0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::GO, 4, 2, 3));
	ectx.storeMem<MXUTetra>(0x1000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0x6024, ectx.readReg<MXUOcta>(4));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x1000);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeReg<MXUOcta>(4, 0x0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::GOI, 4, 2, 0x10));
	ectx.storeMem<MXUTetra>(0x1000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0x6024, ectx.readReg<MXUOcta>(4));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BN, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BN, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BZ, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BZ, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BP, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BP, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BOD, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BOD, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNN, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNN, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNZ, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNZ, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNP, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BNP, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BEV, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BEV, 2, 0, 4));
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BZB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BZB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BPB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BPB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BODB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BODB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNNB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNNB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNZB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNZB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNPB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BNPB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BEVB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::BEVB, 2, 0xff, 0xfc));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0x6, 0x66));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x1000);
	ectx.storeReg<MXUOcta>(3, 0x10);
	ectx.storeReg<MXUOcta>(4, 0x0);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::PUSHGO, 4, 2, 3));
	ectx.storeMem<MXUTetra>(0x1000, 0x10, makeOpcode(MmixOpcode::SETL, 0, 0x6, 0x66));
	ectx.storeMem<MXUTetra>(0x1000, 0x14, makeOpcode(MmixOpcode::SETL, 1, 0x13, 0x13));
	ectx.storeMem<MXUTetra>(0x1000, 0x18, makeOpcode(MmixOpcode::POP, 2, 0, 0));
	stepInto(ectx, 0x6020); ectx.setHalt(false);
	assertEqualsU(0x1313, ectx.readReg<MXUOcta>(4));
	assertEqualsU(0x666, ectx.readReg<MXUOcta>(5));
}
