#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testLogicOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::AND, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0000000000000000, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDI, 1, 2, 0xff));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x00000000000000AA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::OR, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFFFF, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORI, 1, 2, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAAFF, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xFFFFFFFFFFFFFFFF);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::XOR, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x5555555555555555, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::XORI, 1, 2, 0xFF));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAA55, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xAAAAAAAAAAAAAAAA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDNI, 1, 2, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORN, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xAAAAAAAAAAAAAAAA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORNI, 1, 2, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFFAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NAND, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFFFF, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xAAAAAAAAAAAAAAAA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NANDI, 1, 2, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFF55, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NOR, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0000000000000000, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xAAAAAAAAAAAAAAAA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NORI, 1, 2, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x5555555555555500, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xFFFFFFFFFFFFFFFF);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NXOR, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0xAAAAAAAAAAAAAAAA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NXORI, 1, 2, 0xFF));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x55555555555555AA, ectx.readReg<MXUOcta>(1));

	ectx.storeSpReg(SpecialReg::rM, 0xFFFFFFFF00000000);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MUX, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAA55555555, ectx.readReg<MXUOcta>(1));

	ectx.storeSpReg(SpecialReg::rM, 0xFFFFFFFF00000000);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MUXI, 1, 2, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAA00000055, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SADD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(32, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x55AA55AA55AA55AA);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SADD, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(16, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(3, 0x5555555555555555);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SADDI, 1, 2, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(28, ectx.readReg<MXUOcta>(1));
}
