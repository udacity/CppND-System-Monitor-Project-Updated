#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unistd.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : uptime_(LinuxParser::UpTime()) {
  for (auto cpuStat : LinuxParser::AllJiffies()) {
    cpu_.push_back(Processor(cpuStat.cpuID, cpuStat.activeJiffies, cpuStat.totalJiffies));
  }
  for (int pid : LinuxParser::Pids()) {
    AddProcess(pid);
  }
  // sort processes on default column
  std::sort(processes_.begin(), processes_.end(), sortCmp_[sortProcess_]);
}

// Return the system's CPU
vector<Processor>& System::Cpu() { return cpu_; }

void System::UpdateStats(const char sortProcess) {
  uptime_ = LinuxParser::UpTime();
  //update cpu
  auto cpuStats = LinuxParser::AllJiffies();
  int len = std::min(cpu_.size(), cpuStats.size());
  for (int i=0; i<len; ++i) {
    if (cpuStats[i].cpuID == cpu_[i].CpuID()) {
      cpu_[i].UpdateUtilization(cpuStats[i].activeJiffies, cpuStats[i].totalJiffies);
    }
  }
  // update processes
  auto pids = LinuxParser::Pids();
  vector<int> ended_pids;
  int idx = 0;
  for (auto& p : processes_) {
    int pid = p.Pid();
    if (pids.find(pid)==pids.end()) {
      ended_pids.push_back(idx);
    }
    else {
      LinuxParser::PidJiffies pj = LinuxParser::ProcessJiffies(pid);
      p.UpdateStats(pj.activeJiffies/cpu_hertz_, uptime_, LinuxParser::Ram(pid));
      pids.erase(pids.find(pid));
    }
    ++idx;
  }
  // remove ended processes
  for (auto it = ended_pids.crbegin();it!=ended_pids.crend();++it) {
    processes_.erase(processes_.begin()+*it);
  }
  // add new processes
  for (int pid : pids) {
    AddProcess(pid);
  }
  // sort process list based on user input
  if (sortProcess != sortProcess_ && sortCmp_.find(sortProcess)!=sortCmp_.end()) {
    sortProcess_ = sortProcess;
  }
  std::sort(processes_.begin(), processes_.end(), sortCmp_[sortProcess_]);
}

void System::AddProcess(const int pid) {
  LinuxParser::PidJiffies pj = LinuxParser::ProcessJiffies(pid);
  processes_.push_back(Process(
      pid,
      UserId2Name(LinuxParser::Uid(pid)),
      LinuxParser::Command(pid),
      pj.activeJiffies/cpu_hertz_,
      uptime_,
      LinuxParser::Ram(pid),
      pj.startTime/cpu_hertz_
    ));
}

string System::UserId2Name(const int uid) {
  auto it = uid_to_name_.find(uid);
  if (it != uid_to_name_.end()) return it->second;
  return std::to_string(uid);
}

// Return a container composed of the system's processes
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
long System::UpTime() const { return uptime_; }

const string System::os_ = LinuxParser::OperatingSystem();

const string System::kernel_ = LinuxParser::Kernel();

const long System::cpu_hertz_ = (sysconf(_SC_CLK_TCK) > 0 ? sysconf(_SC_CLK_TCK) : 1L);

const std::unordered_map<int, string> System::uid_to_name_ = LinuxParser::Usernames();

std::unordered_map<char, std::function<bool(const Process&, const Process&)>> System::sortCmp_ {
    {'c', [] (const Process& p1, const Process& p2) -> bool {return p1.CpuUtilization() < p2.CpuUtilization() || (p1.CpuUtilization()==p2.CpuUtilization() && p1.Ram()>p2.Ram());}},
    {'r', [] (const Process& p1, const Process& p2) -> bool {return p1.Ram() < p2.Ram();}},
    {'m', [] (const Process& p1, const Process& p2) -> bool {return p1.Command() < p2.Command();}},
    {'u', [] (const Process& p1, const Process& p2) -> bool {return p1.User() < p2.User() || (p1.User() == p2.User() && p1.Ram() > p2.Ram());}},
    {'p', [] (const Process& p1, const Process& p2) -> bool {return p1.Pid() < p2.Pid();}},
    {'t', [] (const Process& p1, const Process& p2) -> bool {return p1.StartTime() < p2.StartTime();}},
    {'C', [] (const Process& p1, const Process& p2) -> bool {return p1.CpuUtilization() > p2.CpuUtilization() || (p1.CpuUtilization()==p2.CpuUtilization() && p1.Ram()>p2.Ram());}},
    {'R', [] (const Process& p1, const Process& p2) -> bool {return p1.Ram() > p2.Ram();}},
    {'M', [] (const Process& p1, const Process& p2) -> bool {return p1.Command() > p2.Command();}},
    {'U', [] (const Process& p1, const Process& p2) -> bool {return p1.User() > p2.User() || (p1.User() == p2.User() && p1.Ram() > p2.Ram());}},
    {'P', [] (const Process& p1, const Process& p2) -> bool {return p1.Pid() > p2.Pid();}},
    {'T', [] (const Process& p1, const Process& p2) -> bool {return p1.StartTime() > p2.StartTime();}}
};
