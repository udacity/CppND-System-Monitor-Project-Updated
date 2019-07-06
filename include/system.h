#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include "process.h"
#include "util.h"

class System {
 public:
  System();
  std::string MemoryUtilization() const;
  long UpTime() const;
  std::string TotalProcesses() const;
  std::string RunningProcesses() const;
  std::string Kernel() const;
  std::string OperatingSystem() const;
  std::string AggregateCpuUtilization();
  std::vector<std::string> IndividualCpuUtilizations();
  std::vector<Process> Processes() const;

 private:
  void UpdateProcesses();  // TODO: Move to ProcessParser
  std::vector<Process> processes_;
  std::vector<std::string> cached_aggregate_cpu_times_;
  std::vector<std::vector<std::string>> cached_individual_cpu_times_;
  std::vector<std::vector<std::string>> cached_cpu_times_;
};

#endif