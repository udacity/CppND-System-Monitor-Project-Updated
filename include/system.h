#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  std::vector<Processor>& Cpu();      // See src/system.cpp
  void UpdateStats();                 // See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // See src/system.cpp
  int TotalProcesses();               // See src/system.cpp
  int RunningProcesses();             // See src/system.cpp
  static std::string Kernel();         // See src/system.cpp
  static std::string OperatingSystem();// See src/system.cpp

  // TODO: Define any necessary private members
 private:
  static const std::string os_;
  static const std::string kernel_;
  std::vector<Processor> cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif
