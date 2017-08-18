#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testLoadMmo(MmixLlvm::ECtx& ectx) {
	const wchar_t* argv[] = { L"Foo", L"Bar", L"Baz", nullptr };
	bool succ = MmixLlvm::loadMmoFile(DR(ectx), L"hello.mmo", argv);
	assertEqualsU(true, succ);
	assertEqualsU(0x8fff0100u, ectx.readMem<MXUTetra>(0, 0x100));
	assertEqualsU(0x00000701u, ectx.readMem<MXUTetra>(0, 0x104));
	assertEqualsU(0xf4ff0003u, ectx.readMem<MXUTetra>(0, 0x108));
	assertEqualsU(0x00000701u, ectx.readMem<MXUTetra>(0, 0x10c));
	assertEqualsU(0x00000000u, ectx.readMem<MXUTetra>(0, 0x110));
	assertEqualsU(0x2c20776fu, ectx.readMem<MXUTetra>(0, 0x114));
	assertEqualsU(0x726c640au, ectx.readMem<MXUTetra>(0, 0x118));
	assertEqualsU(ectx.readReg<MXUOcta>(0), 4);

	std::vector<std::string> test_argv;
	MXUOcta argvpp = ectx.readReg<MXUOcta>(1);
	MXUOcta argvp;
	do {
		argvp = ectx.readMem<MXUOcta>(0, argvpp);
		if (argvp != 0) {
			test_argv.push_back(readString(ectx, 0, argvp));
			argvpp += 8;
		}
	} while (argvp != 0);

	assertTrue(test_argv[0] == "hello.mmo");
	assertTrue(test_argv[1] == "Foo");
	assertTrue(test_argv[2] == "Bar");
	assertTrue(test_argv[3] == "Baz");

	succ = MmixLlvm::loadMmoFile(DR(ectx), L"hardex.mmo");
	assertEqualsU(true, succ);
}