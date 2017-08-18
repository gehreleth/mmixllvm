#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testArithOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUB, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-2, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUB, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::max(), ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -5);
	ectx.storeReg<MXUOcta>(3, -7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUB, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(2, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 666);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUBI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-2, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUBI, 1, 2, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::max(), ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -5);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SUBI, 1, 2, -7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(2, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADD, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(12, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, -1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADD, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::max(), ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -5);
	ectx.storeReg<MXUOcta>(3, -7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADD, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-12, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 666);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADDI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(12, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADDI, 1, 2, -1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::max(), ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -5);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ADDI, 1, 2, -7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-12, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEG, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, std::numeric_limits<MXSOcta>::min());
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEG, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::min(), ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);
	ectx.storeReg<MXUOcta>(1, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGI, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, std::numeric_limits<MXSOcta>::min());
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGI, 1, 0, -128));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(128, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGU, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, -1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGU, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGUI, 1, 0, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGUI, 1, 0, 128));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-128, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::NEGUI, 1, 0, 0xff));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-0xff, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_2ADDU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(17, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_2ADDUI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(17, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_4ADDU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(27, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_4ADDUI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(27, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_8ADDU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(47, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_8ADDUI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(47, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_16ADDU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(87, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 5);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::_16ADDUI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(87, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 10);
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::DIV, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(3, ectx.readReg<MXUOcta>(1));
	assertEqualsU(1, ectx.readSpReg(SpecialReg::rR));

  ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 1699);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::DIVI, 1, 2, 17));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(99, ectx.readReg<MXUOcta>(1));
	assertEqualsU(16, ectx.readSpReg(SpecialReg::rR));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 2);
	ectx.storeSpReg(SpecialReg::rD, 1);
	disableFlags(ectx);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::DIVU, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::min(), ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rR));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeSpReg(SpecialReg::rD, 1);
	disableFlags(ectx);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::DIVUI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(std::numeric_limits<MXSOcta>::min(), ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rR));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 10);
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MUL, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(30, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MUL, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 10);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MULI, 1, 2, 4));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(40, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MULI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x9e3779b97f4a7c16);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MULU, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x61c8864680b583ea, ectx.readSpReg(SpecialReg::rH));
	assertEqualsU(0x1bb32095ccdd51e4, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MULUI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rH));
	assertEqualsU(4, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SL, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(-16, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rA));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SL, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SLI, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(-16, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rA));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SLI, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(0, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SR, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(-1, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rA));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SR, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(std::numeric_limits<MXSOcta>::min() / 8, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -2);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SRI, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(-1, ectx.readReg<MXUOcta>(1));
	assertEqualsU(0, ectx.readSpReg(SpecialReg::rA));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SRI, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsS(std::numeric_limits<MXSOcta>::min() / 8, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 3);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SLU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-8, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SLUI, 1, 2, 4));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-16, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 1);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SRU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1ULL << 62, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, std::numeric_limits<MXSOcta>::min());
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SRUI, 1, 2, 1));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1ULL << 62, ectx.readReg<MXUOcta>(1));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) == 0);

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CMP, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CMPI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(-1, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 7);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CMPU, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, -1);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::CMPUI, 1, 2, 7));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222222222);
	ectx.storeReg<MXUOcta>(3, 0x1133113311331133);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BDIF, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x1100110011001100, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222222222);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::BDIFI, 1, 2, 0x33));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x2222222222222200, ectx.readReg<MXUOcta>(1));


	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222222222);
	ectx.storeReg<MXUOcta>(3, 0x1111333311113333);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::WDIF, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x1111000011110000, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222220022);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::WDIFI, 1, 2, 0x11));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x2222222222220011, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222222222);
	ectx.storeReg<MXUOcta>(3, 0x1111111133333333);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::TDIF, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x1111111100000000, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222200000022);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::TDIFI, 1, 2, 0x11));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x2222222200000011, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222222222222);
	ectx.storeReg<MXUOcta>(3, 0x1111111133333333);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ODIF, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x11111110eeeeeeef, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x2222222200000022);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ODIFI, 1, 2, 0x11));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x2222222200000011, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x0123456789abcdefULL);
	ectx.storeReg<MXUOcta>(3, 0x0102040810204080ULL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MOR, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xefcdab8967452301, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0x0123456789abcdefULL);
	ectx.storeReg<MXUOcta>(3, 0x0102040810204080ULL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::MXOR, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xefcdab8967452301, ectx.readReg<MXUOcta>(1));
}
