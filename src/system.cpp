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

  // Update pids and create a map
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

// std::vector<Process> System::Processes() {
//   vector<Process> processes;
//   map<int, long> process_jiffies;
//   long system_jiffies = UpTime() * sysconf(_SC_CLK_TCK);
//   for (auto& pid : Pids()) {
//     processes.emplace_back(pid);
//     process_jiffies[pid] = processes.back().Jiffies();
//     if (cached_process_jiffies_.find(pid) != cached_process_jiffies_.end()) {
//       float cpu = static_cast<float>(process_jiffies[pid] -
//                                      cached_process_jiffies_[pid]) /
//                   static_cast<float>(system_jiffies -
//                   cached_system_jiffies_);
//       processes.back().CpuUtilization(cpu);
//     }
//   }
//   std::sort(processes.begin(), processes.end(), std::greater<Process>());
//   cached_process_jiffies_ = process_jiffies;
//   cached_system_jiffies_ = system_jiffies;
//   return processes;
// }

std::string System::Kernel() const {
  string token;
  for (string& line : LinuxParser::Lines(LinuxParser::kProcDirectory +
                                         LinuxParser::kVersionFilename)) {
    std::istringstream stream(line);
    for (int i = 0; i <= 2; ++i) {
      stream >> token;
    }
  }
  return token;
}

float System::MemoryUtilization() const {
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() const {
  string key, value;
  for (string& line : LinuxParser::Lines("/etc/os-release")) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "PRETTY_NAME") {
        std::replace(value.begin(), value.end(), '_', ' ');
        return value;
      }
    }
  }
  return "";
}

int System::RunningProcesses() const {
  string key, value;
  for (string& line : LinuxParser::Lines(LinuxParser::kProcDirectory +
                                         LinuxParser::kStatFilename)) {
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }
  return 0;
}

int System::TotalProcesses() const {
  string key, value;
  for (string& line : LinuxParser::Lines(LinuxParser::kProcDirectory +
                                         LinuxParser::kStatFilename)) {
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}

long int System::UpTime() const {
  string token;
  for (string& line : LinuxParser::Lines(LinuxParser::kProcDirectory +
                                         LinuxParser::kUptimeFilename)) {
    std::istringstream stream(line);
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}