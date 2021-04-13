#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  for (auto cpuStat : LinuxParser::AllJiffies()) {
    cpu_.push_back(Processor(cpuStat.cpuID, cpuStat.activeJiffies, cpuStat.totalJiffies));
  }
}

// Return the system's CPU
// refreshes cpu utilization first
// assuming order of cpus does not change 
vector<Processor>& System::Cpu() { return cpu_; }

void System::UpdateStats() {
  auto cpuStats = LinuxParser::AllJiffies();
  int len = std::min(cpu_.size(), cpuStats.size());
  for (int i=0; i<len; ++i) {
    if (cpuStats[i].cpuID == cpu_[i].CpuID()) {
      cpu_[i].UpdateUtilization(cpuStats[i].activeJiffies, cpuStats[i].totalJiffies);
    }
  }
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
string System::Kernel() { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
string System::OperatingSystem() { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

const string System::os_ = LinuxParser::OperatingSystem();

const string System::kernel_ = LinuxParser::Kernel();

