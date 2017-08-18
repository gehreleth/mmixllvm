#include "stdafx.h"
#include <mmoloader.h>
#include <interpreter.h>

using namespace MmixLlvm;

void testRudimentaryIo(MmixLlvm::ECtx& ectx) {
	char testStr[] = "Some text";
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0, MmixLlvm::POOL_SEG + 16);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 8, static_cast<MXUByte>(RIO_Mode::TextWrite));
	storeString(ectx, MmixLlvm::POOL_SEG, 16, "test.txt");
	storeString(ectx, MmixLlvm::POOL_SEG, 0x100, testStr);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fopen), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG + 0x100);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fputs), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(0, ectx.readReg<MXUOcta>(1));
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fclose), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0, MmixLlvm::POOL_SEG + 16);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 8, static_cast<MXUByte>(RIO_Mode::TextRead));
	storeString(ectx, MmixLlvm::POOL_SEG, 16, "test.txt");
	storeString(ectx, MmixLlvm::POOL_SEG, 0x100, testStr);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fopen), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x100, 0x3000);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x108, 512);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG + 0x100);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fread), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(ectx.readReg<MXUOcta>(1), strlen(testStr) - 512);
	std::string readStr = readString(ectx, 0, 0x3000);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fclose), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertTrue(strcmp(testStr, readStr.c_str()) == 0);

	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0, MmixLlvm::POOL_SEG + 16);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 8, static_cast<MXUByte>(RIO_Mode::TextRead));
	storeString(ectx, MmixLlvm::POOL_SEG, 16, "test.txt");
	storeString(ectx, MmixLlvm::POOL_SEG, 0x100, testStr);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fopen), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x100, 0x3000);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x108, 512);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG + 0x100);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fgets), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(ectx.readReg<MXUOcta>(1), strlen(testStr) + 1 /* Strlen Doesn't include \0 */);
	readStr = readString(ectx, 0, 0x3000);

	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Ftell), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(ectx.readReg<MXUOcta>(1), strlen(testStr));

	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x100, -2);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG + 0x100);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fseek), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(ectx.readReg<MXUOcta>(1), 0);

	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Ftell), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertEqualsU(ectx.readReg<MXUOcta>(1), strlen(testStr) - 1);

	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fclose), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertTrue(strcmp(testStr, readStr.c_str()) == 0);

	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0, MmixLlvm::POOL_SEG + 16);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 8, static_cast<MXUByte>(RIO_Mode::TextRead));
	storeString(ectx, MmixLlvm::POOL_SEG, 16, "test.txt");
	storeString(ectx, MmixLlvm::POOL_SEG, 0x100, testStr);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fopen), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x100, 0x3000);
	ectx.storeMem<MXUOcta>(MmixLlvm::POOL_SEG, 0x108, 512);
	ectx.storeReg<MXUOcta>(255, MmixLlvm::POOL_SEG + 0x100);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fgetws), 5));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);

	ectx.storeReg<MXUOcta>(255, 0x3000);
	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fputws), 1));
	ectx.storeMem<MXUTetra>(0x1004, 0x0, makeOpcode(MmixOpcode::ORI, 1, 255, 0));
	stepInto(ectx, 0x1000); ectx.setHalt(false);

	ectx.storeMem<MXUTetra>(0x1000, 0x0, makeOpcode(MmixOpcode::TRAP, 0, static_cast<MXUByte>(RIO_Opc::Fclose), 5));
	stepInto(ectx, 0x1000); ectx.setHalt(false);
	assertTrue(strcmp(testStr, readStr.c_str()) == 0);
}
