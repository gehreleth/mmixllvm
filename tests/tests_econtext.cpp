#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testStoreAndLoad(MmixLlvm::ECtx& ctx) {
	MXUOcta uo1 = 0x0123456789abcdefULL;
	ctx.storeMem(MmixLlvm::STACK_SEG, 2, uo1);
	assertEqualsU(ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 2), ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	assertEqualsU(0x45, ctx.readMem<MXUByte>(MmixLlvm::STACK_SEG, 2));

	assertEqualsU(0x4567, ctx.readMem<MXUWyde>(MmixLlvm::STACK_SEG, 2));

	assertEqualsU(0x01234567, ctx.readMem<MXUTetra>(MmixLlvm::STACK_SEG, 2));

	assertEqualsU(0x0123456789abcdefULL, ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 2));

	assertEqualsS(ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 5), ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	assertEqualsS(0xffffffffffffffabULL, (MXSByte)ctx.readMem<MXUByte>(MmixLlvm::STACK_SEG, 5));

	assertEqualsS(0xffffffffffff89abULL, (MXSWyde)ctx.readMem<MXUWyde>(MmixLlvm::STACK_SEG, 5));

	assertEqualsS(0xffffffff89abcdefULL, (MXSTetra)ctx.readMem<MXUTetra>(MmixLlvm::STACK_SEG, 5));

	assertEqualsS(0x0123456789abcdefULL, (MXSOcta)ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 5));

	MXSOcta so1 = -65536;
	ctx.storeMem<MXUByte>(MmixLlvm::STACK_SEG, 2, so1);
	assertEqualsU(0x0123006789abcdefULL, ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	ctx.storeMem(MmixLlvm::STACK_SEG, 2, uo1);
	ctx.storeMem<MXUWyde>(MmixLlvm::STACK_SEG, 2, so1);
	assertEqualsU(0x0123000089abcdefULL, ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	ctx.storeMem(MmixLlvm::STACK_SEG, 2, uo1);
	ctx.storeMem<MXUTetra>(MmixLlvm::STACK_SEG, 2, so1);
	assertEqualsU(0xffff000089abcdefULL, ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	ctx.storeMem(MmixLlvm::STACK_SEG, 2, uo1);
	ctx.storeMem(MmixLlvm::STACK_SEG, 2, so1);
	assertEqualsU(so1, ctx.readMem<MXUOcta>(MmixLlvm::STACK_SEG, 0));

	std::vector<char> test(100);
	for (auto q = test.begin(); q != test.end(); ++q) {
		*q = (char)(rand() % 256);
	}
	ctx.importExternalBuffer(&test[0], 1017, test.size());
	int ec = 0;
	for (int i = 0; i < test.size(); i++) {
		char c0 = test[i];
		char c1 = ctx.readMem<MXUByte>(1017, i);
		if (c0 != c1) {
			++ec;
		}
	}
	assertTrue(ec == 0);
	ec = 0;
	std::vector<char> test1(100);
	ctx.exportExternalBuffer(1017, &test1[0], 100);
	for (int i = 0; i < test1.size(); i++) {
		char c0 = test1[i];
		char c1 = ctx.readMem<MXUByte>(1017, i);
		if (c0 != c1) {
			++ec;
		}
	}
	assertTrue(ec == 0);
}

void testStack(MmixLlvm::ECtx& ctx) {
	for (MXUByte r = 0; r < 5; r++) {
		ctx.storeReg<MXUOcta>(r, r);
	}
	assertEqualsU(1, ctx.readReg<MXUOcta>(1));
	assertEqualsU(5, ctx.readSpReg(SpecialReg::rL));
	ctx.push(5);
	ctx.pop(0);
	ctx.push(5);
	ctx.storeReg<MXUOcta>(0, 13);
	ctx.pop(2);
	assertEqualsU(13, ctx.readReg<MXUOcta>(5));
	for (MXUByte r = 0; r < 5; r++) {
		assertEqualsU(r, ctx.readReg<MXUOcta>(r));
	}
	assertEqualsU(6, ctx.readSpReg(SpecialReg::rL));
	assertEqualsU(0, ctx.readSpReg(SpecialReg::rO));
	srand(time(nullptr));
	std::stack<MXFp> control;
	MXFp ceil = double(RAND_MAX);
	int ec = 0;
	for (int i = 0; i < 10000; i++) {
		for (MXUByte r = 0; r < 10; r++) {
			MXFp rn = MXFp(rand()) / ceil;
			ctx.storeReg<MXFp>(r, rn);
			control.push(rn);
		}
		ctx.push(10);
	}
	for (int i = 0; i < 10000; i++) {
		ctx.pop(0);
		for (MXUByte r0 = 0; r0 < 10; r0++) {
			MXUByte r = 9 - r0;
			MXFp cn = control.top();
			control.pop();
			MXFp an = ctx.readReg<MXFp>(r);
			if (cn != an) {
				++ec;
			}
		}
	}
	assertEqualsU(0, ec);
}
