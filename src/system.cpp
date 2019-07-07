#include <dirent.h>
#include <unistd.h>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "process.h"
#include "system.h"
#include "linux_parser.h"

using std::map;
using std::size_t;
using std::string;
using std::vector;



vector<int> System::Pids() const {
  vector<int> pids;
  DIR* directory = opendir("/proc");
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

std::vector<Process> System::Processes() {
  vector<Process> processes;
  map<int, long> process_jiffies;
  long system_jiffies = UpTime() * sysconf(_SC_CLK_TCK);
  for (auto& pid : Pids()) {
    processes.emplace_back(pid);
    process_jiffies[pid] = processes.back().Jiffies();
    if (cached_process_jiffies_.find(pid) != cached_process_jiffies_.end()) {
      float cpu = static_cast<float>(process_jiffies[pid] -
                                     cached_process_jiffies_[pid]) /
                  static_cast<float>(system_jiffies - cached_system_jiffies_);
      processes.back().CpuUtilization(cpu);
    }
  }
  std::sort(processes.begin(), processes.end(), std::greater<Process>());
  cached_process_jiffies_ = process_jiffies;
  cached_system_jiffies_ = system_jiffies;
  return processes;
}

vector<float> System::IndividualCpuUtilizations() {
  vector<float> utilizations;
  vector<vector<string>> individual_cpu_times =
      LinuxParser::IndividualCpuUtilizations();
  for (size_t i = 0; i < individual_cpu_times.size(); ++i) {
    utilizations.push_back(0.0);
    if (i < cached_individual_cpu_times_.size() &&
        cached_individual_cpu_times_[i].size() > 0) {
      utilizations[i] = LinuxParser::CpuUtilization(
          cached_individual_cpu_times_[i], individual_cpu_times[i]);
    }
  }
  cached_individual_cpu_times_ = individual_cpu_times;
  return utilizations;
}

float System::AggregateCpuUtilization() {
  vector<string> cpu_times = LinuxParser::AggregateCpuUtilization();
  float utilization{0};
  if (cached_aggregate_cpu_times_.size() > 0) {
    utilization =
        LinuxParser::CpuUtilization(cached_aggregate_cpu_times_, cpu_times);
  }
  cached_aggregate_cpu_times_ = cpu_times;
  return utilization;
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

std::string System::Kernel() const {
  string token;
  for (string& line : LinuxParser::Lines(Path::base + Path::version)) {
    std::istringstream stream(line);
    for (int i = 0; i <= 2; ++i) {
      stream >> token;
    }
  }
  return token;
}

long int System::UpTime() const {
  string token;
  for (string& line : LinuxParser::Lines(Path::base + Path::uptime)) {
    std::istringstream stream(line);
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}

int System::TotalProcesses() const {
  string key, value;
  for (string& line : LinuxParser::Lines(Path::base + Path::stat)) {
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}

int System::RunningProcesses() const {
  string key, value;
  for (string& line : LinuxParser::Lines(Path::base + Path::stat)) {
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}