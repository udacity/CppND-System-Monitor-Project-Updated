#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include "process_parser.h"
#include "util.h"

class System {
 public:
  System();
  void Refresh();
  void setLastCpuMeasures();
  std::string MemoryUtilization() const;
  long UpTime() const;
  std::string getThreads() const;
  std::string getTotalProc() const;
  std::string getRunningProc() const;
  std::string Kernel() const;
  std::string OperatingSystem() const;
  std::string Cpu() const;
  void getOtherCores(int size);
  void setCpuCoresStats();
  std::vector<std::string> getCoresStats() const;

 private:
  std::vector<std::string> lastCpuStats;
  std::vector<std::string> currentCpuStats;
  std::vector<std::string> coresStats;
  std::vector<std::vector<std::string>> lastCpuCoresStats;
  std::vector<std::vector<std::string>> currentCpuCoresStats;
  std::string cpu_{-1};
  float memPercent;
  std::string os_name_;
  std::string kernel_version_;
  long up_time_;
  int total_procs_;
  int running_proc_;
  int threads_;
};

#endif