#ifndef ERROR_HANDLING_H_INCLUDED
#define ERROR_HANDLING_H_INCLUDED

#include <cstdint>
#include <exception>
#include <string>
#include <sstream>
#include <windows.h>
#include "misc.h"

#define MMIXERROR(...) MmixLlvm::onError(__FILE__, __LINE__, __VA_ARGS__)

namespace MmixLlvm {

class Exception : public std::exception {
  const std::string file_;
  const int line_;
  const std::string message_;
public:
  Exception(const char* file, int line, const char* message);
  virtual const char* what();
  virtual const char* getFile();
  virtual int getLine();
  virtual ~Exception();
};

struct ExceptionStrategy {
  static void onError(const char* file, int line, const char* message);
};

struct CerrStrategy {
  static void onError(const char* file, int line, const char* message);
};

#pragma pack (push, 1)
struct SehError {
  char fileName[MAX_PATH];
  int line;
  char message[256];
};
#pragma pack (pop)

template<uint32_t SEH_ERROR = STATUS_USER_APC>
struct SehStrategy {
  static void onError(const char* file, int line, const char* message);
};

template <uint32_t SEH_ERROR>
void SehStrategy<SEH_ERROR>::onError(const char* file, int line, const char* message)
{
  SehError se;
  memset(&se, 0, sizeof(se));
  se.line = line;
  enum { MsgArrSize = sizeof(se.message) };
  strcpy_s(se.message, MsgArrSize, message);
  enum { FnArrSize = sizeof(se.fileName) };
  strcpy_s(se.fileName, MsgArrSize, file);
  SehError *sehStruct[] = { &se };
  RaiseException(SEH_ERROR, 0, 1, (const ULONG_PTR*)sehStruct);
}

typedef SehStrategy<STATUS_USER_APC> EffectiveErrStrategy;

namespace EHPrivate {
template <typename First, typename ... Rest>
void cc(std::wstringstream& s, First f, Rest ... r)
{
  s << f << L' ';
  cc(s, r ...);
};

template <typename First>
void cc(std::wstringstream& s, First f)
{
  s << f;
};
}

template<typename ... Args>
void onError(const char* file, int line, Args ... args)
{
  std::wstringstream s;
  EHPrivate::cc(s, args ...);
  std::string msg = toMultibyte(s.str().c_str());
  EffectiveErrStrategy::onError(file, line, msg.c_str());
}

};

#endif