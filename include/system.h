#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  std::vector<Processor>& Cpu();                       // See src/system.cpp
  void UpdateStats(const char sortProcess);            // See src/system.cpp
  std::vector<Process>& Processes();                   // See src/system.cpp
  float MemoryUtilization();                           // See src/system.cpp
  long UpTime() const;                                 // See src/system.cpp
  int TotalProcesses();                                // See src/system.cpp
  int RunningProcesses();                              // See src/system.cpp
  static std::string Kernel();                         // See src/system.cpp
  static std::string OperatingSystem();                // See src/system.cpp
  static std::string UserId2Name(const int uid);       // See src/system.cpp

  // Define any necessary private members
 private:
  static const std::string os_;
  static const std::string kernel_;
  static const long cpu_hertz_;          // clock ticks per second
  static const std::unordered_map<int, std::string> uid_to_name_;
  static std::unordered_map<char, std::function<bool(const Process&,const Process&)>> sortCmp_;
  char sortProcess_ = 'C';  // c,C - cpu, r,R - ram, t,T - uptime, m,M - command, u,U - user, p,P - pid
  long uptime_ = 0L;
  std::vector<Processor> cpu_ = {};
  std::vector<Process> processes_ = {};
  void AddProcess(const int pid);
};

#endif
