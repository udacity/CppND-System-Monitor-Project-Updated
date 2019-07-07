#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process> Processes();
  float MemoryUtilization() const;
  long UpTime() const;
  int TotalProcesses() const;
  int RunningProcesses() const;
  std::string Kernel() const;
  std::string OperatingSystem() const;

 private:
  Processor cpu_;
  std::vector<int> Pids() const;
  std::map<int, long> cached_process_jiffies_;
  long cached_system_jiffies_{0};
};

#endif