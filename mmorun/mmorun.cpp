// mmorun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <rudimentary_io.h>
#include <mmo_reader_dest_ref.h>
#include <jit.h>

using namespace MmixLlvm;

namespace {
	MmixLlvm::VaMemStrategy memStrategy;

	MmixLlvm::ECtx* makeContext() {
		return new MmixLlvm::ExecutionContext(memStrategy.getSegs());
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

	MXUTetra makeOpcode(MmixOpcode opc, MXUByte x, MXUByte y, MXUByte z) {
		return (static_cast<MXUTetra>(opc) << 24) | (x << 16) | (y << 8) | z;
	}

	int main0(MmixLlvm::ECtx& ectx, int argc, const wchar_t** argv) {
		for (MXUByte i = 0; i <= 8; i++) {
			MXUOcta base = i << 4;
			ectx.storeMem<MXUTetra>(base, 0, makeOpcode(MmixOpcode::TRAP, 1, 0, 0));
			ectx.storeMem<MXUTetra>(base, 4, makeOpcode(MmixOpcode::GET, 255, 0, static_cast<MXUByte>(SpecialReg::rB)));
			ectx.storeMem<MXUTetra>(base, 8, makeOpcode(MmixOpcode::RESUME, 0, 0, 0));
		}
		MmixLlvm::WinRudimentaryIoImpl rio;
		ectx.registerSyscallHandler(&rio);
		const wchar_t *objFileName = nullptr;
		if (argc >= 2) {
			objFileName = argv[1];
		} else {
			std::wcerr << L"Usage :" << argv[0] << "<mmo file> [arguments]\n";
			return 2;
		}
//		MmixLlvm::ExecGraphBuilder::SymTab symTab;

		/*class MmoLoaderDestRef0 : public MmoLoaderDestRef {
			MmixLlvm::ExecGraphBuilder::SymTab& symTab_;
		public:
			MmoLoaderDestRef0(ExecutionContext& ctx, MmixLlvm::ExecGraphBuilder::SymTab& symTab)
				:MmoLoaderDestRef(ctx)
				,symTab_(symTab)
			{}
			virtual void setSymbol(const char* symbol, MXUOcta equiv) {
				symTab_[equiv] = symbol;
				MmoLoaderDestRef::setSymbol(symbol, equiv);
			}
		} DR(ectx, symTab); */

		MmoLoaderDestRef DR(ectx);
		//HANDLE h = CreateEvent(NULL, TRUE, FALSE, NULL);
		//MmixLlvm::JitContext jctx(ectx);
		if (MmixLlvm::loadMmoFile(DR, objFileName, argv + 2)) {
			//jctx.start();
			stepInto(ectx, ectx.getXNextPtr());
			//WaitForSingleObject(h, INFINITE);
			return 0;
		} else {
			return 2;
		}
	}
};

int wmain(int argc, const wchar_t** argv) {
	MmixLlvm::ECtx* theEctx = nullptr;
	int retVal = 1;
	__try {
		__try {
			theEctx = makeContext();
			retVal = main0(*theEctx, argc, argv);
		}
		__except (sehFilter(GetExceptionCode(), GetExceptionInformation(), theEctx))
		{ }
	}
	__finally {
		destroyContext(theEctx);
	}
	return retVal;
}

