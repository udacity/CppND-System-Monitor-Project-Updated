#include <unistd.h>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "linux_system.h"
#include "process.h"
#include "processor.h"

using std::map;
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& LinuxSystem::Cpu() { return cpu_; }

vector<Process>& LinuxSystem::Processes() {
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

std::string LinuxSystem::Kernel() { return LinuxParser::Kernel(); }

float LinuxSystem::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

std::string LinuxSystem::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

int LinuxSystem::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int LinuxSystem::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int LinuxSystem::UpTime() { return LinuxParser::UpTime(); }