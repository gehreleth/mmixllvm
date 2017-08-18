#ifndef INTERPRETER_COMMON_INCLUDED
#define INTERPRETER_COMMON_INCLUDED

#include "mmixdef.h"
#include "econtext.h"

namespace MmixLlvm {

namespace InterpreterPrivate {

inline MXUByte x(MXUTetra opc) {
  return static_cast<MXUByte>((opc >> 16) & 0xff);
}

inline MXUByte y(MXUTetra opc) {
  return static_cast<MXUByte>((opc >> 8) & 0xff);
}

inline MXUByte zu(MXUTetra opc) {
  return static_cast<MXUByte>(opc & 0xff);
}

inline MXSByte zs(MXUTetra opc) {
  return static_cast<MXSByte>(opc & 0xff);
}

inline MXUOcta yzu(MXUTetra opc) {
  return static_cast<MXUWyde>(opc & 0xffff);
}

inline MXSOcta yzs(MXUTetra opc) {
  return static_cast<MXSWyde>(opc & 0xffff);
}

inline MXSTetra xyzof(MXUTetra opc) {
  return static_cast<MXSTetra>(opc & 0x00ffffff);
}

inline MXSTetra xyzob(MXUTetra opc) {
  return static_cast<MXSTetra>(opc | 0xff000000);
}

inline MXSTetra yzof(MXUTetra opc) {
  return static_cast<MXSTetra>(opc & 0x0000ffff);
}

inline MXSTetra yzob(MXUTetra opc) {
  return static_cast<MXSTetra>(opc | 0xffff0000);
}

inline MXSOcta s(ExecutionContext& ctx, MXUByte reg) {
  return ctx.readReg<MXSOcta>(reg);
}

inline MXUOcta u(ExecutionContext& ctx, MXUByte reg) {
  return ctx.readReg<MXUOcta>(reg);
}

inline MXFp f(ExecutionContext& ctx, MXUByte reg) {
  return ctx.readReg<MXFp>(reg);
}

inline RoundMode roundMode(ExecutionContext& ctx, MXUByte y) {
  return static_cast<RoundMode>(y != 0 ? y : ctx.flags().bits.roundMode + 1);
}

inline bool ncond(MXSOcta arg) {
  return arg < 0;
}

inline bool zcond(MXSOcta arg) {
  return arg == 0;
}

inline bool pcond(MXSOcta arg) {
  return arg > 0;
}

inline bool nncond(MXSOcta arg) {
  return arg >= 0;
}

inline bool nzcond(MXSOcta arg) {
  return arg != 0;
}

inline bool npcond(MXSOcta arg) {
  return arg <= 0;
}

inline bool evcond(MXSOcta arg) {
  return (arg & 1) == 0;
}

inline bool odcond(MXSOcta arg) {
  return (arg & 1) != 0;
}

};

};

#endif
