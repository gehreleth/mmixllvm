#ifndef STATS_COLLECTOR_H_DEFINED
#define STATS_COLLECTOR_H_DEFINED

#include "mmixdef.h"

namespace MmixLlvm {
class ExecutionContext;

class StatsCollector {
public:
  virtual void traceInit(MXUOcta to) = 0;
  virtual void traceJump(MXUOcta from, MXUOcta to) = 0;
  virtual void traceCondJump(MXUOcta from, MXUOcta to) = 0;
  virtual void traceCall(MXUOcta from, MXUOcta to) = 0;
  virtual void tracePop(MXUOcta from, MXUOcta to) = 0;
  virtual void traceTrip(MXUOcta from, MXUOcta to) = 0;
  virtual void traceTrap(MXUOcta from, MXUOcta to) = 0;
  virtual void traceResumeFromTrip(MXUOcta from, MXUOcta to) = 0;
  virtual void traceResumeFromTrap(MXUOcta from, MXUOcta to) = 0;
  virtual ~StatsCollector() = default;
};

}

#endif
