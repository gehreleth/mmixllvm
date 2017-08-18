#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testLoadOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeReg<MXUOcta>(2, 1000);
	ectx.storeReg<MXUOcta>(3, 2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDB, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x45, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDW, 1, 2, 3));
	stepInto(ectx, 0x6010);; ectx.setHalt(false);
	assertEqualsU(0x4567, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDT, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x01234567, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDO, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123456789abcdefUL, ectx.readReg<MXUOcta>(1));
	ectx.storeReg<MXUOcta>(3, 5);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDB, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffffffffffabUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDW, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffffffff89abUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDT, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffff89abcdefUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDHT, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x89abcdef00000000UL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDHTI, 1, 2, 5));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x89abcdef00000000UL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDO, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123456789abcdefUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDBI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x45, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDWI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x4567, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDTI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x01234567, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDOI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123456789abcdefUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDBI, 1, 2, 5));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffffffffffabUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDWI, 1, 2, 5));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffffffff89abUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDTI, 1, 2, 5));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xffffffff89abcdefUL, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::LDOI, 1, 2, 5));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123456789abcdefUL, ectx.readReg<MXUOcta>(1));
}

void testStoreOpcopes(MmixLlvm::ECtx& ectx) {
	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeReg<MXUOcta>(1, -65536);
	ectx.storeReg<MXUOcta>(2, 1000);
	ectx.storeReg<MXUOcta>(3, 2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STB, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123006789abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STW, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123000089abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STT, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFF000089abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue(!(ectx.readSpReg(SpecialReg::rA) & V));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STO, 1, 2, 3));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFF0000UL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue(!(ectx.readSpReg(SpecialReg::rA) & V));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STBI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123006789abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STWI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123000089abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue((ectx.readSpReg(SpecialReg::rA) & V) != 0);

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STTI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFF000089abcdefUL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue(!(ectx.readSpReg(SpecialReg::rA) & V));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STOI, 1, 2, 2));
	disableFlags(ectx);
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFF0000UL, ectx.readMem<MXUOcta>(0, 1000));
	assertTrue(!(ectx.readSpReg(SpecialReg::rA) & V));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeReg<MXUOcta>(1, 0xFFFFFFFFFFFFFFFFUL);
	ectx.storeReg<MXUOcta>(2, 1000);
	ectx.storeReg<MXUOcta>(3, 2);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STBU, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123FF6789abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STWU, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123FFFF89abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STTU, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFF89abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STOU, 1, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFFFFUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STBUI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123FF6789abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STWUI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0123FFFF89abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STTUI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFF89abcdefUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STOUI, 1, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFFFFFFFFFFFFFUL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STCO, 0x13, 2, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0000000000000013UL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeMem(0, 1000, 0x0123456789abcdefUL);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::STCOI, 0x13, 2, 2));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0000000000000013UL, ectx.readMem<MXUOcta>(0, 1000));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETH, 1, 0, 1));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1ULL << 48, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETMH, 1, 0, 1));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1ULL << 32, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETML, 1, 0, 1));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1ULL << 16, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::SETL, 1, 0, 1));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(1, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORH, 1, 0x55, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xFFFFAAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORMH, 1, 0x55, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAFFFFAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORML, 1, 0x55, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAFFFFAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ORL, 1, 0x55, 0x55));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAAFFFF, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDNH, 1, 0xAA, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x0000AAAAAAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDNMH, 1, 0xAA, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAA0000AAAAAAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDNML, 1, 0xAA, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAA0000AAAA, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0xAAAAAAAAAAAAAAAA);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::ANDNL, 1, 0xAA, 0xAA));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0xAAAAAAAAAAAA0000, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 13);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::PUT, static_cast<MXUByte>(SpecialReg::rH), 0, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(13, ectx.readSpReg(SpecialReg::rH));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::PUTI, static_cast<MXUByte>(SpecialReg::rH), 0, 14));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(14, ectx.readSpReg(SpecialReg::rH));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::GET, 1, 0, static_cast<MXUByte>(SpecialReg::rH)));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(14, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::GETA, 1, 0, 3));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x6010 + 12, ectx.readReg<MXUOcta>(1));

	ectx.storeReg<MXUOcta>(1, 0);
	ectx.storeReg<MXUOcta>(2, 0);
	ectx.storeReg<MXUOcta>(3, 0);
	ectx.storeMem<MXUTetra>(0x6000, 0x10, makeOpcode(MmixOpcode::GETAB, 1, 0xFF, 0xFE));
	stepInto(ectx, 0x6010); ectx.setHalt(false);
	assertEqualsU(0x6010 - 8, ectx.readReg<MXUOcta>(1));
}
