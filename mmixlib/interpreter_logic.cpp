#include "stdafx.h"
#include "mmixdef.h"
#include "econtext.h"
#include "interpreter_common.h"
#include "interpreter_logic.h"

using namespace MmixLlvm;
using namespace MmixLlvm::InterpreterPrivate;

namespace {

MXUOcta bdifImpl(ExecutionContext& ctx, MXUOcta lho, MXUOcta rho)
{
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } q;
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } w;
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } r;
  q.val = lho;
  w.val = rho;
  r.a = _mm_subs_epu8(q.a, w.a);
  return r.val;
}

MXUOcta wdifImpl(ExecutionContext& ctx, MXUOcta lho, MXUOcta rho)
{
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } q;
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } w;
  __declspec(align(16)) union {
    __m128i a;
    MXUOcta val;
  } r;
  q.val = lho;
  w.val = rho;
  r.a = _mm_subs_epu16(q.a, w.a);
  return r.val;
}

MXUOcta tdifImpl(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  MXUOcta t[2] = {
    static_cast<MXUOcta>(std::max<MXSOcta>(0, static_cast<MXSOcta>((lho & 0xffffffff)) - static_cast<MXSOcta>(rho & 0xffffffff))),
    static_cast<MXUOcta>(std::max<MXSOcta>(0, static_cast<MXSOcta>(lho >> 32) - static_cast<MXSOcta>(rho >> 32)))
  };
  return (static_cast<MXUOcta>(t[1]) << 32) | static_cast<MXUOcta>(t[0]);
}

MXUOcta odifImpl(ExecutionContext& ctx, MXSOcta lho, MXSOcta rho)
{
  return lho > rho ? lho - rho : 0;
}

MXUOcta morImpl(MXUOcta y, MXUOcta z)
{
  MXUOcta x = 0;
#define morl(i) \
		x |= (((z >> (i)) & 0x0101010101010101ull) * 0xff) & ((y >> ((i) << 3) & 0xff) * 0x0101010101010101ull);

  morl(0);
  morl(1);
  morl(2);
  morl(3);
  morl(4);
  morl(5);
  morl(6);
  morl(7);
#undef morl
  return x;
}

MXUOcta mxorImpl(MXUOcta y, MXUOcta z)
{
  MXUOcta x = 0;
#define mxorl(i) \
		x ^= (((z >> (i)) & 0x0101010101010101ull) * 0xff) & ((y >> ((i) << 3) & 0xff) * 0x0101010101010101ull);

  mxorl(0);
  mxorl(1);
  mxorl(2);
  mxorl(3);
  mxorl(4);
  mxorl(5);
  mxorl(6);
  mxorl(7);
#undef mxorl
  return x;
}
};

void MmixLlvm::InterpreterPrivate::and_(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) & u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::andi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) & zu(opc));
}

void MmixLlvm::InterpreterPrivate::or_(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) | u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::ori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) | zu(opc));
}

void MmixLlvm::InterpreterPrivate::xor_(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) ^ u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::xori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) ^ zu(opc));
}

void MmixLlvm::InterpreterPrivate::andn(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) & ~u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::andni(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) & ~(zu(opc)));
}

void MmixLlvm::InterpreterPrivate::orn(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) | ~u(ctx, zu(opc)));
}

void MmixLlvm::InterpreterPrivate::orni(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), u(ctx, y(opc)) | ~(zu(opc)));
}

void MmixLlvm::InterpreterPrivate::nand(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) & u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::nandi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) & zu(opc)));
}

void MmixLlvm::InterpreterPrivate::nor(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) | u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::nori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) | zu(opc)));
}

void MmixLlvm::InterpreterPrivate::nxor(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) ^ u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::nxori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), ~(u(ctx, y(opc)) ^ zu(opc)));
}

void MmixLlvm::InterpreterPrivate::mux(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta mux = ctx.readSpReg(SpecialReg::rM);
  ctx.storeReg(x(opc), ((u(ctx, y(opc)) & mux)) | (u(ctx, zu(opc)) & ~mux));
}

void MmixLlvm::InterpreterPrivate::muxi(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta mux = ctx.readSpReg(SpecialReg::rM);
  ctx.storeReg(x(opc), ((u(ctx, y(opc)) & mux)) | (zu(opc) & ~mux));
}

void MmixLlvm::InterpreterPrivate::sadd(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta v = u(ctx, y(opc)) & ~u(ctx, zu(opc));
  MXUByte c;
  for (c = 0; v; c++) {
    v &= v - 1;
  }
  ctx.storeReg<MXUOcta>(x(opc), c);
}

void MmixLlvm::InterpreterPrivate::saddi(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta v = u(ctx, y(opc)) & ~zu(opc);
  MXUByte c;
  for (c = 0; v; c++) {
    v &= v - 1;
  }
  ctx.storeReg<MXUOcta>(x(opc), c);
}

void MmixLlvm::InterpreterPrivate::orh(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal | (yzu(opc) << 48));
}

void MmixLlvm::InterpreterPrivate::ormh(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal | (yzu(opc) << 32));
}

void MmixLlvm::InterpreterPrivate::orml(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal | (yzu(opc) << 16));
}

void MmixLlvm::InterpreterPrivate::orl(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal | yzu(opc));
}

void MmixLlvm::InterpreterPrivate::andnh(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal & ~(yzu(opc) << 48));
}

void MmixLlvm::InterpreterPrivate::andnmh(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal & ~(yzu(opc) << 32));
}

void MmixLlvm::InterpreterPrivate::andnml(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal & ~(yzu(opc) << 16));
}

void MmixLlvm::InterpreterPrivate::andnl(ExecutionContext& ctx, MXUTetra opc)
{
  MXUOcta xVal = u(ctx, x(opc));
  ctx.storeReg(x(opc), xVal & ~yzu(opc));
}

void MmixLlvm::InterpreterPrivate::bdif(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), bdifImpl(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::bdifi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), bdifImpl(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::wdif(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), wdifImpl(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::wdifi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), wdifImpl(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::tdif(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), tdifImpl(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::tdifi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), tdifImpl(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::odif(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), odifImpl(ctx, u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::odifi(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), odifImpl(ctx, u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::mor(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), morImpl(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::mori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), morImpl(u(ctx, y(opc)), zu(opc)));
}

void MmixLlvm::InterpreterPrivate::mxor(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), mxorImpl(u(ctx, y(opc)), u(ctx, zu(opc))));
}

void MmixLlvm::InterpreterPrivate::mxori(ExecutionContext& ctx, MXUTetra opc)
{
  ctx.storeReg(x(opc), mxorImpl(u(ctx, y(opc)), zu(opc)));
}
