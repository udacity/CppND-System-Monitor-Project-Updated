#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "process.h"

class System {
 public:
  std::vector<Process> Processes();
  float MemoryUtilization() const;
  long UpTime() const;
  int TotalProcesses() const;
  int RunningProcesses() const;
  std::string Kernel() const;
  std::string OperatingSystem() const;
  float AggregateCpuUtilization();
  std::vector<float> IndividualCpuUtilizations();

 private:
  std::vector<int> Pids() const;
  std::vector<std::string> cached_aggregate_cpu_times_;
  std::vector<std::vector<std::string>> cached_individual_cpu_times_;
  std::map<int, long> cached_process_jiffies_;
  long cached_system_jiffies_{0};
};

#endif