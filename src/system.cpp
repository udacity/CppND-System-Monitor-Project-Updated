#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  for (std::vector<int>::iterator it = pids.begin(); it != pids.end(); it++) {
    int pidIndex = *it;
    std::string userVal = LinuxParser::User(pidIndex);
    std::string commandVal = LinuxParser::Command(pidIndex);
    std::string ramVal = LinuxParser::Ram(pidIndex);
    long upTimeVal = LinuxParser::UpTime(pidIndex);
    std::string uidVal = LinuxParser::Uid(pidIndex);
    vector<string> cpuUtilVale = LinuxParser::CpuUtilization();
    Process process(pidIndex);      
    processes_.push_back(process); 
  };

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }