// tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>
#include <rudimentary_io.h>
#include <cost_graph.h>

using namespace MmixLlvm;

int errCounter = 0;

int succCounter = 0;

void assertTrue(bool arg, const char* errMessage) {
	if (!arg) {
		fputs(errMessage, stderr);
		++errCounter;
	} else {
		fputs("Success !", stdout);
		++succCounter;
	}
	fputs("\n", stderr);
}

void assertEqualsU(MXUOcta expected, MXUOcta actual, const char* errMessage) {
	if (expected != actual) {
		fprintf(stderr, "%s Expected=%016llX, actual=%016llX", errMessage, expected, actual);
		++errCounter;
	} else {
		fputs("Success !", stdout);
		++succCounter;
	}
	fputs("\n", stderr);
}

void assertEqualsS(MXSOcta expected, MXSOcta actual, const char* errMessage) {
	if (expected != actual) {
		fprintf(stderr, "%s Expected=%016llX, actual=%016llX", errMessage, expected, actual);
		++errCounter;
	} else {
		fputs("Success !", stdout);
		++succCounter;
	}
	fputs("\n", stderr);
}

void assertEqualsF(MmixLlvm::MXFp expected, MmixLlvm::MXFp actual, const char* errMessage) {
	if (expected != actual) {
		fprintf(stderr, "%s Expected=%d, actual=%d", errMessage, expected, actual);
		++errCounter;
	}
	else {
		fputs("Success !", stdout);
		++succCounter;
	}
	fputs("\n", stderr);
}

void disableFlags(MmixLlvm::ECtx& ctx) {
	MXUOcta ra = ctx.readSpReg(SpecialReg::rA);
	ra &= 0xff00;
	ctx.storeSpReg(SpecialReg::rA, ra);
}

MXUTetra makeOpcode(MmixOpcode opc, MXUByte x, MXUByte y, MXUByte z) {
	return (static_cast<MXUByte>(opc) << 24) | (x << 16) | (y << 8) | z;
}

void storeString(MmixLlvm::ECtx& ectx, MXUOcta base, MXUOcta offset, const char* arg) {
	const char* c = arg;
	while (char c0 = *c++) {
		ectx.storeMem<MXUByte>(base, offset++, c0);
	}
	if ((offset & 7) == 0) {
		ectx.storeMem<MXUByte>(base, offset++, 0);
	}
	while ((offset & 7) != 0) {
		ectx.storeMem<MXUByte>(base, offset++, 0);
	}
}

std::string readString(MmixLlvm::ECtx& ectx, MXUOcta base, MXUOcta offset) {
	std::string retVal;
	size_t i = 0;
	MXUByte b;
	MXUOcta base0 = base + offset;
	while ((b = ectx.readMem<MXUByte>(base + offset, i++)) != 0) {
		retVal.push_back(static_cast<char>(b));
	}
	return retVal;
}

int main0(MmixLlvm::ECtx& ectx, int argc, _TCHAR* argv[]) {
	MmixLlvm::WinRudimentaryIoImpl rio;
	ectx.registerSyscallHandler(&rio);

	for (MXUByte i = 0; i <= 8; i++) {
		MXUOcta base = i << 4;
		ectx.storeMem<MXUTetra>(base, 0, makeOpcode(MmixOpcode::TRAP, 1, 0, 0));
		ectx.storeMem<MXUTetra>(base, 4, makeOpcode(MmixOpcode::GET, 255, 0, static_cast<MXUByte>(SpecialReg::rB)));
		ectx.storeMem<MXUTetra>(base, 8, makeOpcode(MmixOpcode::RESUME, 0, 0, 0));
	}
	ectx.setHalt(false);

	storeString(ectx, MmixLlvm::POOL_SEG, 0, "Test output\n");
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG);
	ectx.storeMem<MXUTetra>(0x6000, 0x20, makeOpcode(MmixOpcode::TRAP, 0, 7, 1));
	ectx.storeMem<MXUTetra>(0x6000, 0x24, 0);
	stepInto(ectx, 0x6020);ectx.setHalt(false);

	bool succ = MmixLlvm::loadMmoFile(DR(ectx), L"primes.mmo");
	assertEqualsU(true, succ);
	stepInto(ectx, ectx.getXNextPtr());ectx.setHalt(false);

	succ = MmixLlvm::loadMmoFile(DR(ectx), L"hardex.mmo");
	assertEqualsU(true, succ);
  testJit(ectx);
	//testCostGraph(ectx); // ToDo: make normal, pedintic tests
	testArithFlag(ectx);
	testStoreAndLoad(ectx);
	testStack(ectx);
	testLoadMmo(ectx);
	testLoadOpcopes(ectx);
	testStoreOpcopes(ectx);
	testArithOpcopes(ectx);
	testCondOpcopes(ectx);
	testLogicOpcopes(ectx);
	testGotoOpcopes(ectx);
	testFpOpcopes(ectx);
	testRudimentaryIo(ectx);
	return 0;
}

namespace {
	MmixLlvm::VaMemStrategy memStrategy;

	MmixLlvm::ECtx* makeContext() {
		return new MmixLlvm::ECtx(memStrategy.getSegs());
	}

	void destroyContext(MmixLlvm::ECtx* arg) {
		delete arg;
	}

	DWORD sehFilter(DWORD code, struct _EXCEPTION_POINTERS *ptrs, MmixLlvm::ECtx* ectx) {
		switch (code) {
		case EXCEPTION_ACCESS_VIOLATION:
			return MmixLlvm::VaMemStrategy::tryLazyCommitPage(&memStrategy, ptrs);
		case STATUS_USER_APC:
			{
				MmixLlvm::SehError *err = (MmixLlvm::SehError *) ptrs->ExceptionRecord->ExceptionInformation[0];
				MessageBoxA(nullptr, err->message, "Error", MB_OK | MB_ICONERROR);
			}
			return EXCEPTION_EXECUTE_HANDLER;
		default:
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
};

int _tmain(int argc, _TCHAR* argv[]) {
	MmixLlvm::ECtx* theEctx = nullptr;
	int retVal = 1;
	__try {
		__try {
			theEctx = makeContext();
			retVal = main0(*theEctx, argc, argv);
		} __except(sehFilter(GetExceptionCode(), GetExceptionInformation(), theEctx))
		{ } 
	} __finally {
		destroyContext(theEctx);
	}
	if (errCounter == 0) {
		printf("All tests passed");
	}
	else {
		fprintf(stderr, "Success %i, fail %i", succCounter, errCounter);
	}
	return retVal;
}

