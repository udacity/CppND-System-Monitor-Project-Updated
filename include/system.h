#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include "process.h"

class System {
 public:
  float MemoryUtilization() const;
  long UpTime() const;
  int TotalProcesses() const;
  int RunningProcesses() const;
  std::string Kernel() const;
  std::string OperatingSystem() const;
  float AggregateCpuUtilization();
  std::vector<float> IndividualCpuUtilizations();
  std::vector<Process> Processes() const;

 private:
  std::vector<std::string> cached_aggregate_cpu_times_;
  std::vector<std::vector<std::string>> cached_individual_cpu_times_;
};

#endif