#ifndef MMO_LOADER_INCLUDED
#define MMO_LOADER_INCLUDED

#include "mmixdef.h"

namespace MmixLlvm {

struct MmoLoaderDest {
  virtual void setArgc(MXUOcta val) = 0;

  virtual void setArgv(MXUOcta val) = 0;

  virtual void setG(MXUByte val) = 0;

  virtual MXUByte getG() = 0;

  virtual void setGreg(MXUByte greg, MXUOcta val) = 0;

  virtual void setTimestamp(time_t arg) = 0;

  virtual MXUOcta fixOcta(MXUOcta base, MXUOcta offset) = 0;

  virtual MXUTetra fixTetra(MXUOcta base, MXUOcta offset) = 0;

  virtual void setOcta(MXUOcta base, MXUOcta offset, MXUOcta arg) = 0;

  virtual void setTetra(MXUOcta base, MXUOcta offset, MXUTetra arg) = 0;

  virtual void setWyde(MXUOcta base, MXUOcta offset, MXUWyde arg) = 0;

  virtual void setByte(MXUOcta base, MXUOcta offset, MXUByte arg) = 0;

  virtual void setSymbol(const char* symbol, MXUOcta equiv) = 0;

  virtual ~MmoLoaderDest() = 0;
};

bool loadMmoFile(MmoLoaderDest& dest, const wchar_t* fileName, const wchar_t** argv = nullptr);

};

#endif