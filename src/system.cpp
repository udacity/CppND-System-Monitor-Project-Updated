#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using namespace std;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  std::sort(pids.begin(), pids.end());
  /* using emplace back for performance and simplicity as per reviewers note */
  for (const int& pid : pids) {
    processes_.emplace_back(pid);
  }

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
  ;
}

long int System::UpTime() { return LinuxParser::UpTime(); }