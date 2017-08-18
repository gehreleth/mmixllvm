// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#define NOMINMAX

#include "targetver.h"

// TODO: reference additional headers your program requires here
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cfenv>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <atomic>
#include <array>
#include <thread>
#include <llvm/ADT/GraphTraits.h>
#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/LazyEmittingLayer.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>

