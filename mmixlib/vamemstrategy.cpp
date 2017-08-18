#include "stdafx.h"
#include "vamemstretegy.h"

using namespace MmixLlvm;

VaMemStrategy::VaMemStrategy()
  :_pageSize(getPageSize())
{
  const SIZE_T physBytesToAllocate = 256 * 1024UL * 1024UL;
  const SIZE_T segmentSize = physBytesToAllocate / SEG_COUNT;
  _baseMem = VirtualAlloc(NULL, physBytesToAllocate, MEM_RESERVE | MEM_TOP_DOWN, PAGE_NOACCESS);
  for (int i = 0; i < SEG_COUNT; i++) {
    _segs[i] = static_cast<MXUByte*>(_baseMem) + i*segmentSize;
  }
}

VaMemStrategy::~VaMemStrategy()
{
  VirtualFree(_baseMem, 0, MEM_RELEASE);
}

DWORD VaMemStrategy::tryLazyCommitPage(VaMemStrategy* this_, EXCEPTION_POINTERS *ptrs)
{
  if (this_ != NULL) {
    ULONG_PTR ptr = ptrs->ExceptionRecord->ExceptionInformation[1];
    LPVOID ptrR = reinterpret_cast<void*>(ptr & ~(this_->_pageSize - 1));
    if (ptrR >= this_->_baseMem) {
      ptrR = VirtualAlloc(ptrR, this_->_pageSize, MEM_COMMIT, PAGE_READWRITE);
      return ptrR != NULL ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
    } else {
      return EXCEPTION_CONTINUE_SEARCH;
    }
  } else {
    return EXCEPTION_CONTINUE_SEARCH;
  }
}

DWORD VaMemStrategy::getPageSize()
{
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwPageSize;
}

void** VaMemStrategy::getSegs()
{
  return _segs;
}
