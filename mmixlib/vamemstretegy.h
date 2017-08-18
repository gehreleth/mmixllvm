#ifndef VA_MEM_STRATEGY_H_INCLUDED
#define VA_MEM_STRATEGY_H_INCLUDED

#include "mmixdef.h"
#include "econtext.h"
#include "errorhandling.h"
#include <windows.h>

namespace MmixLlvm {
	class VaMemStrategy {
		enum {SEG_COUNT = 4};
		void* _segs[SEG_COUNT];
		LPVOID _baseMem;
		const size_t _pageSize;
		static DWORD getPageSize();
	public:
		VaMemStrategy();
		static DWORD tryLazyCommitPage(VaMemStrategy* this_, EXCEPTION_POINTERS *ptrs);
		~VaMemStrategy();
		void** getSegs();
	};
};

#endif //VA_MEM_STRATEGY_H_INCLUDED