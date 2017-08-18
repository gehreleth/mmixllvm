#include "stdafx.h"
#include "errorhandling.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace MmixLlvm;

Exception::Exception(const char* file, int line, const char* message)
  : file_(file)
  , line_(line)
  , message_(message)
{}

Exception::~Exception() {}

const char* Exception::what()
{
  return message_.c_str();
}

const char* Exception::getFile()
{
  return file_.c_str();
}

int Exception::getLine()
{
  return line_;
}

void ExceptionStrategy::onError(const char* file, int line, const char* message)
{
  throw Exception(file, line, message);
}

void CerrStrategy::onError(const char* file, int line, const char* message)
{
  std::cerr << "Error in file \""
    << file << ", line "
    << line 
    << " message: \"" << message << "\"";
}

template SehStrategy<STATUS_USER_APC>;