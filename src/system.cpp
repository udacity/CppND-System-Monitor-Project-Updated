#include <unistd.h>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::map;
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids{LinuxParser::Pids()};

  // Create a set
  set<int> extant_pids;
  for (Process const& process : processes_) {
    extant_pids.insert(process.Pid());
  }

  // Emplace all new processes
  for (int pid : pids) {
    if (extant_pids.find(pid) == extant_pids.end())
      processes_.emplace_back(pid);
  }

  // Update CPU utilization
  for (auto& process : processes_) {
    process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()),
                           LinuxParser::Jiffies());
  }

  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

std::string System::Kernel() const { return LinuxParser::Kernel(); }

float System::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() const {
  return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }