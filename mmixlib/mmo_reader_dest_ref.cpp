#include "stdafx.h"
#include "mmo_reader_dest_ref.h"
#include "econtext.h"

using namespace MmixLlvm;

MmoLoaderDestRef::MmoLoaderDestRef(ExecutionContext& ctx)
  :ctx_(ctx) {}

void MmoLoaderDestRef::setArgc(MXUOcta val) {
  ctx_.storeReg(0, val);
}

void MmoLoaderDestRef::setArgv(MXUOcta val) {
  ctx_.storeReg(1, val);
}

void MmoLoaderDestRef::setG(MXUByte val) {
  ctx_.storeSpReg(SpecialReg::rG, val);
}

MXUByte MmoLoaderDestRef::getG() {
  return static_cast<MXUByte>(ctx_.readSpReg(SpecialReg::rG));
}

void MmoLoaderDestRef::setGreg(MXUByte greg, MXUOcta val) {
  ctx_.storeReg(greg, val);
}

void MmoLoaderDestRef::setTimestamp(time_t arg) {
  // no timestamp
}

MXUOcta MmoLoaderDestRef::fixOcta(MXUOcta base, MXUOcta offset) {
  return ctx_.readMem<MXUOcta>(base, offset);
}

MXUTetra MmoLoaderDestRef::fixTetra(MXUOcta base, MXUOcta offset) {
  return ctx_.readMem<MXUTetra>(base, offset);
}

void MmoLoaderDestRef::setOcta(MXUOcta base, MXUOcta offset, MXUOcta arg) {
  ctx_.storeMem<MXUOcta>(base, offset, arg);
}

void MmoLoaderDestRef::setTetra(MXUOcta base, MXUOcta offset, MXUTetra arg) {
  ctx_.storeMem<MXUTetra>(base, offset, arg);
}

void MmoLoaderDestRef::setWyde(MXUOcta base, MXUOcta offset, MXUWyde arg) {
  ctx_.storeMem<MXUWyde>(base, offset, arg);
}

void MmoLoaderDestRef::setByte(MXUOcta base, MXUOcta offset, MXUByte arg) {
  ctx_.storeMem<MXUByte>(base, offset, arg);
}

void MmoLoaderDestRef::setSymbol(const char* symbol, MXUOcta equiv) {
  if (strcmp("Main", symbol) == 0) {
    ctx_.setEntryPoint(equiv);
    ctx_.setHalt(false);
  }
}

MmoLoaderDestRef::~MmoLoaderDestRef() {}
