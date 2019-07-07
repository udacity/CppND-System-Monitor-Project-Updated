#ifndef LINUX_SYSTEM_H
#define LINUX_SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

class LinuxSystem : public System {
 public:
  Processor& Cpu() override;
  std::vector<Process>& Processes() override;
  float MemoryUtilization() override;
  long UpTime() override;
  int TotalProcesses() override;
  int RunningProcesses() override;
  std::string Kernel() override;
  std::string OperatingSystem() override;

 private:
  Processor cpu_;
  std::vector<Process> processes_;
};

#endif