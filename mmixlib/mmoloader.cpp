#include "stdafx.h"
#include "mmoloader.h"
#include "errorhandling.h"
#include <iostream>
#include <fstream>

using namespace MmixLlvm;

namespace {
const char* CANT_OPEN_FILE = "Can't open file";

const char* ILLEGAL_LOP_PARAM = "Illegal Lop param";

const char* UNEXPECTED_EOF = "Unexpected EOF";

const char* UNKNOWN_LOP_CODE = "Unknown Lop_code";

const char* CANT_READ_PREAMBLE = "Can't read preamble";

const char* CANT_READ_BODY = "Can't read body";

const char* CANT_READ_POSTAMBLE = "Can't read postamble";

const char* CANT_READ_SYMBOLS = "Can't read symbols";

const char* SYMBOL_TOO_LONG = "The symbol is too long";

const char* UNEXPECTED_LOP = "Unexpected lop";

enum LopCode {
  Lop_Quote = 0, Lop_Loc = 1, Lop_Skip = 2, Lop_Fixo = 3,
  Lop_Fixr = 4, Lop_Fixrx = 5, Lop_File = 6, Lop_Line = 7,
  Lop_Spec = 8, Lop_Pre = 9, Lop_Post = 0xa, Lop_Stab = 0xb,
  Lop_End = 0xc,
  Lop_Ill = 0xff
};

enum { Mm = 0x98 };

MXUTetra tt(const MXUByte arg[])
{
  return arg[3] | (arg[2] << 8) | (arg[1] << 16) | (arg[0] << 24);
}

bool isLop(MXUTetra arg)
{
  return Mm == ((arg >> 24) & 0xff);
}

LopCode getLopCode(MXUTetra arg)
{
  LopCode lo = (LopCode)((arg >> 16) & 0xff);
  switch (lo) {
    case Lop_Quote:
    case Lop_Loc:
    case Lop_Skip:
    case Lop_Fixo:
    case Lop_Fixr:
    case Lop_Fixrx:
    case Lop_File:
    case Lop_Line:
    case Lop_Spec:
    case Lop_Pre:
    case Lop_Post:
    case Lop_Stab:
    case Lop_End:
      return lo;
    default:
      return Lop_Ill;
  }
}

MXUWyde getYZ(MXUTetra arg)
{
  return static_cast<MXUWyde>(arg & 0xffff);
}

MXUByte getY(MXUTetra arg)
{
  return static_cast<MXUByte>((arg >> 8) & 0xff);
}

MXUByte getZ(MXUTetra arg)
{
  return static_cast<MXUByte>(arg & 0xff);
}

MXUOcta incr(MXUOcta arg, MXSTetra delta)
{
  bool neg = delta < 0;
  MXUTetra d = static_cast<MXUTetra>(neg ? -delta : delta);
  return !neg ? arg + d : arg - d;
}

struct ReadContext {
  time_t fileCreated;
  bool postamble;
  MXUOcta lambda;
  MXUByte fileno;
  MXUWyde lineno;
  char *sym_ptr;
  char sym_buf[128];
  MXUOcta equiv;
  MXUOcta entryPoint;
};

bool readTetra(std::istream& is, MXUTetra& out)
{
  MXUByte buf[4];
  is.read((char*)&buf[0], sizeof(buf));
  if (!is.fail()) {
    out = tt(buf);
    return true;
  } else
    return false;
}

bool readByte(std::istream& is, MXUByte& out)
{
  MXUByte buf[1];
  is.read((char*)&buf[0], sizeof(buf));
  if (!is.fail()) {
    out = buf[0];
    return true;
  } else
    return false;
}

bool readPreamble(std::istream& is, MmoLoaderDest& dest, ReadContext& rctx)
{
  MXUTetra t;
  if (!readTetra(is, t)) {
    MMIXERROR( UNEXPECTED_EOF, "offset", is.tellg());
    return false;
  }
  switch (getLopCode(t)) {
    case Lop_Pre:
      if (getY(t) == 1) {
        MXUByte preabmleSize = getZ(t);
        for (int i = 0; i < preabmleSize; i++) {
          if (!readTetra(is, t)) {
            MMIXERROR( UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          // First entry is time, the rest of preamble is user-defined
          if (i == 0) {
            dest.setTimestamp(t);
          }
        }
        return true;
      } else
        return false;
    default:
      return false;
  }
}

bool readBody(std::istream& is, MmoLoaderDest& dest, ReadContext& rctx)
{
  MXUTetra t;
  if (!readTetra(is, t)) {
    MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
    return false;
  }
  while (!rctx.postamble) {
    if (isLop(t)) {
      switch (getLopCode(t)) {
        case Lop_Quote:
          if (getYZ(t) != 1) {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          break;
        case Lop_Loc: {
          MXUTetra hiLoc;
          MXUTetra loLoc;
          MXUByte y = getY(t);
          if (getZ(t) == 2) {
            if (!readTetra(is, t)) {
              MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
              return false;
            }
            hiLoc = (y << 24) + t;
          } else if (getZ(t) == 1) {
            hiLoc = y << 24;
          } else {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          loLoc = t;
          rctx.lambda = (static_cast<MXUOcta>(hiLoc) << 32) | loLoc;
        }
                      if (!readTetra(is, t)) {
                        MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
                        return false;
                      }
                      continue;
        case Lop_Skip:
          rctx.lambda = incr(rctx.lambda, static_cast<MXSWyde>(getYZ(t)));
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          continue;
        case Lop_Fixo:
        {
          MXUTetra hiLoc;
          MXUTetra loLoc;
          MXUByte y = getY(t);
          if (getZ(t) == 2) {
            if (!readTetra(is, t)) {
              MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
              return false;
            }
            hiLoc = (y << 24) + t;
          } else if (getZ(t) == 1) {
            hiLoc = y << 24;
          } else {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          loLoc = t;
          MXUOcta fixAddr = (static_cast<MXUOcta>(hiLoc) << 32) | loLoc;
          //ectx.storeMem(fixAddr, 0, ectx.readMem<MXUOcta>(rctx.lambda, 0));
          dest.setOcta(fixAddr, 0, dest.fixOcta(rctx.lambda, 0));
        }
        if (!readTetra(is, t)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        continue;
        case Lop_Fixr:
        {
          MXUTetra yz = getYZ(t);
          MXUOcta P = incr(rctx.lambda, -(static_cast<MXSTetra>(getYZ(t)) << 2));
          MXUTetra orig = dest.fixTetra(P, 0);
          dest.setTetra(P, 0, orig ^ yz);
        }
        if (!readTetra(is, t)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        continue;
        case Lop_Fixrx:
        {
          MXSTetra j = static_cast<MXSTetra>(getYZ(t));
          if (j != 16 && j != 24) {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          if (t & 0xfe000000) {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
          MXUTetra delta = t;
          MXSTetra delta0 = -static_cast<MXSTetra>(delta >= 1000000 ? (delta & 0xffffff) - (1 << j) : delta) << 2;
          MXUOcta P = incr(rctx.lambda, delta0);
          MXUTetra orig = dest.fixTetra(P, 0);
          dest.setTetra(P, 0, orig ^ delta);
        }
        if (!readTetra(is, t)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        continue;
        case Lop_File:
        {
          rctx.lineno = 0;
          rctx.fileno = getY(t);
          MXUByte fileNameLength = getZ(t);
          for (int i = 0; i < fileNameLength; i++) {
            if (!readTetra(is, t)) {
              MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
              return false;
            }
          }
        }
        if (!readTetra(is, t)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        continue;
        case Lop_Line:
          rctx.lineno = getYZ(t);
          if (!readTetra(is, t)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          continue;
        case Lop_Post:
        {
          MXUByte y = getY(t);
          MXUByte z = getZ(t);
          if (!y && z >= 32) {
            rctx.postamble = true;
            dest.setG(z);
          } else {
            MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
            return false;
          }
        }
        continue;
        case Lop_Spec:
        {
          bool nonQuoteLopcodeFound = false;
          while (!nonQuoteLopcodeFound) {
            if (!readTetra(is, t)) {
              MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
              return false;
            }
            if (isLop(t)) {
              if (getLopCode(t) != Lop_Quote) {
                nonQuoteLopcodeFound = true;
                continue;
              } else if (getYZ(t) == 1) {
                if (!readTetra(is, t)) {
                  MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
                  return false;
                }
              } else {
                MMIXERROR(ILLEGAL_LOP_PARAM, "offset", is.tellg(), "lopc", getLopCode(t), getYZ(t));
                return false;
              }
            }
            rctx.lambda += 4;
          }
        }
        continue;
        default:
          MMIXERROR(UNKNOWN_LOP_CODE, "offset", is.tellg(), "lopc", (t >> 16) & 0xff);
          return false;
      }
    }
    dest.setTetra(rctx.lambda, 0, t);
    rctx.lambda += 4;
    if (!readTetra(is, t)) {
      MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
      return false;
    }
  }
  return true;
}

bool readPostamble(std::istream& is, MmoLoaderDest& dest, ReadContext& rctx)
{
  MXUByte gg = dest.getG();
  int octasToRead = 256 - gg;
  for (MXUByte i = 0; i < octasToRead; i++) {
    MXUTetra th, tl;
    if (!readTetra(is, th)) {
      MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
      return false;
    }
    if (!readTetra(is, tl)) {
      MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
      return false;
    }
    dest.setGreg(gg + i, static_cast<MXUOcta>(th) << 32 | tl);
  }
  return true;
}

bool readSymbols0(std::istream& is, MmoLoaderDest& dest, ReadContext& rctx)
{
  MXUByte b;
  if (!readByte(is, b)) {
    MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
    return false;
  }
  int m = b;
  int c;
  int j, k;
  if (m & 0x40) {
    if (!readSymbols0(is, dest, rctx))
      goto error;
  }
  if (m & 0x2f) {
    if (m & 0x80) {
      if (!readByte(is, b)) {
        MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
        return false;
      }
      j = b;
    } else {
      j = 0;
    }
    if (!readByte(is, b)) {
      MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
      return false;
    }
    c = b;
    if (j) {
      c = '?';
    }
    *rctx.sym_ptr++ = c;
    if (rctx.sym_ptr == &rctx.sym_buf[sizeof(rctx.sym_buf) / sizeof(rctx.sym_buf[0])]) {
      MMIXERROR(SYMBOL_TOO_LONG, is.tellg());
      return false;
    }
    if (m & 0xf) {
      *rctx.sym_ptr = '\0';
      j = m & 0xf;
      if (j == 15) {
        if (!readByte(is, b)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        rctx.equiv = b;
      } else if (j <= 8) {
        rctx.equiv = 0;
        for (; j > 0; j--) {
          if (!readByte(is, b)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          rctx.equiv = (rctx.equiv << 8) | b;
        }
      } else {
        rctx.equiv = 0x20000000000000;
        for (; j > 8; j--) {
          if (!readByte(is, b)) {
            MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
            return false;
          }
          rctx.equiv |= static_cast<MXUOcta>(b) << ((j - 9) << 3);
        }
      }
      if (!readByte(is, b)) {
        MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
        return false;
      }
      k = b;
      j = b;
      bool q = false;
      while (!q) {
        if (k >= 128)
          break;
        if (!readByte(is, b)) {
          MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
          return false;
        }
        k = b;
        j = (j << 7) + k;
      }
      dest.setSymbol(rctx.sym_buf + 1, rctx.equiv);
    }
    if (m & 0x20) {
      if (!readSymbols0(is, dest, rctx))
        goto error;
    }
    rctx.sym_ptr--;
  }
  if (m & 0x10) {
    if (!readSymbols0(is, dest, rctx))
      goto error;
  }
  return true;
error:
  return false;
}

bool readSymbols(std::istream& is, MmoLoaderDest& dest, ReadContext& rctx)
{
  MXUTetra t;
  if (!readTetra(is, t)) {
    MMIXERROR(UNEXPECTED_EOF, "offset", is.tellg());
    return false;
  }
  if (getLopCode(t) != Lop_Stab) {
    MMIXERROR(UNEXPECTED_LOP, "lopc", t, "offset", is.tellg());
    return false;
  }
  rctx.sym_ptr = &rctx.sym_buf[0];
  bool retVal = readSymbols0(is, dest, rctx);
  return retVal;
}

void prepareArgv(MmoLoaderDest& dest, const wchar_t* fileName, const wchar_t** argv)
{
  std::vector<std::string> argv0;
  argv0.push_back(MmixLlvm::toMultibyte(fileName));
  if (argv != nullptr) {
    const wchar_t** q = argv;
    while (*q) {
      argv0.push_back(MmixLlvm::toMultibyte(*q++));
    }
  }
  size_t c = argv0.size();
  size_t argp = 0;
  dest.setArgc(c);
  MXUOcta dataBase = POOL_SEG + 8;
  dest.setArgv(dataBase);
  MXUOcta dataOffset = (c + 2) << 3;
  for (auto itr = argv0.begin(); itr != argv0.end(); ++itr) {
    dest.setOcta(dataBase, argp, dataBase + dataOffset);
    argp += 8;
    std::string& arg0 = *itr;
    for (auto itri = arg0.begin(); itri != arg0.end(); ++itri) {
      dest.setByte(dataBase, dataOffset++, *itri);
    }
    if ((dataOffset & 7) == 0) {
      dest.setByte(dataBase, dataOffset++, 0);
    }
    while ((dataOffset & 7) != 0) {
      dest.setByte(dataBase, dataOffset++, 0);
    }
  }
  dest.setOcta(dataBase, argp, 0);
  dest.setOcta(POOL_SEG, 0, dataBase + dataOffset);
}

};

bool MmixLlvm::loadMmoFile(MmoLoaderDest& dest, const wchar_t* fileName, const wchar_t** argv)
{
  std::ifstream mmo(fileName, std::ios_base::in | std::ios_base::binary);
  if (!mmo.is_open()) {
    MMIXERROR(CANT_OPEN_FILE, fileName);
    return false;
  }

  ReadContext rctx;
  memset(&rctx, 0, sizeof(rctx));

  if (!readPreamble(mmo, dest, rctx)) {
    MMIXERROR(CANT_READ_PREAMBLE);
    return false;
  }

  if (!readBody(mmo, dest, rctx)) {
    MMIXERROR(CANT_READ_BODY);
    return false;
  }

  if (!readPostamble(mmo, dest, rctx)) {
    MMIXERROR(CANT_READ_POSTAMBLE);
    return false;
  }

  if (!readSymbols(mmo, dest, rctx)) {
    MMIXERROR(CANT_READ_SYMBOLS);
    return false;
  }

  prepareArgv(dest, fileName, argv);
  return true;
}

MmoLoaderDest::~MmoLoaderDest() {}
