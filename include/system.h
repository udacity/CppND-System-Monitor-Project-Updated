#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();  // Return the system's CPU
  std::vector<Process>&
  Processes();  // Return a container composed of the system's processes
  float MemoryUtilization();  // Return the system's memory utilization
  long
  UpTime();  // Return the number of seconds since the system started running
  int TotalProcesses();    // Return the total number of processes on the system
  int RunningProcesses();  // Return the number of processes actively running on
                           // the system
  std::string Kernel();    // Return the system's kernel identifier (string)
  std::string OperatingSystem();  // Return the operating system name

 private:
  Processor cpu_;
  std::vector<Process> processes_ = {};
};

#endif