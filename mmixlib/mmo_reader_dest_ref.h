#ifndef MMO_READER_DEST_REF_INCLUDED
#define MMO_READER_DEST_REF_INCLUDED

#include "mmixdef.h"
#include "mmoloader.h"

namespace MmixLlvm {
class ExecutionContext;

class MmoLoaderDestRef : public MmoLoaderDest {
  ExecutionContext& ctx_;
public:
  MmoLoaderDestRef(ExecutionContext& ctx);

  virtual void setArgc(MXUOcta val);

  virtual void setArgv(MXUOcta val);

  virtual void setG(MXUByte val);

  virtual MXUByte getG();

  virtual void setGreg(MXUByte greg, MXUOcta val);

  virtual void setTimestamp(time_t arg);

  virtual MXUOcta fixOcta(MXUOcta base, MXUOcta offset);

  virtual MXUTetra fixTetra(MXUOcta base, MXUOcta offset);

  virtual void setOcta(MXUOcta base, MXUOcta offset, MXUOcta arg);

  virtual void setTetra(MXUOcta base, MXUOcta offset, MXUTetra arg);

  virtual void setWyde(MXUOcta base, MXUOcta offset, MXUWyde arg);

  virtual void setByte(MXUOcta base, MXUOcta offset, MXUByte arg);

  virtual void setSymbol(const char* symbol, MXUOcta equiv);

  virtual ~MmoLoaderDestRef();
};
}

#endif