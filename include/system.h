#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  virtual Processor& Cpu() = 0;
  virtual std::vector<Process>& Processes() = 0;
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual std::string Kernel() = 0;
  virtual std::string OperatingSystem() = 0;
};

#endif