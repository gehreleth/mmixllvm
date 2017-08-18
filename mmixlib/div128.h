#ifndef __DIV_128_H_INCLUDED
#define __DIV_128_H_INCLUDED

extern "C" unsigned __int64 __fastcall udiv128(unsigned __int64 numhi, unsigned __int64 numlo, unsigned __int64 den, unsigned __int64* rem);

extern "C" __int64 __fastcall sdiv128(__int64 numhi, __int64 numlo, __int64 den, __int64* rem);

#endif