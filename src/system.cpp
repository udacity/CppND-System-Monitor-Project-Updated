#include <cstddef>
#include <string>
#include <vector>

#include "process.h"
#include "process_parser.h"
#include "system.h"
#include "system_parser.h"

using std::size_t;
using std::string;
using std::vector;

std::vector<Process> System::Processes() const {
  vector<Process> processes;
  for (auto& pid : ProcessParser::Pids()) {
    processes.emplace_back(pid);
  }
  std::sort(processes.begin(), processes.end(), std::greater<Process>());
  return processes;
}

vector<float> System::IndividualCpuUtilizations() {
  vector<float> utilizations;
  vector<vector<string>> individual_cpu_times =
      SystemParser::IndividualCpuUtilizations();
  for (size_t i = 0; i < individual_cpu_times.size(); ++i) {
    utilizations.push_back(0.0);
    if (i < cached_individual_cpu_times_.size() &&
        cached_individual_cpu_times_[i].size() > 0) {
      utilizations[i] = SystemParser::CpuUtilization(
          cached_individual_cpu_times_[i], individual_cpu_times[i]);
    }
  }
  cached_individual_cpu_times_ = individual_cpu_times;
  return utilizations;
}

float System::AggregateCpuUtilization() {
  vector<string> cpu_times = SystemParser::AggregateCpuUtilization();
  float utilization{0};
  if (cached_aggregate_cpu_times_.size() > 0) {
    utilization =
        SystemParser::CpuUtilization(cached_aggregate_cpu_times_, cpu_times);
  }
  cached_aggregate_cpu_times_ = cpu_times;
  return utilization;
}

float System::MemoryUtilization() const {
  return SystemParser::MemoryUtilization();
}

std::string System::OperatingSystem() const {
  string key, value;
  for (string& line : SystemParser::Lines("/etc/os-release")) {
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
  for (string& line : SystemParser::Lines(Path::base + Path::version)) {
    std::istringstream stream(line);
    for (int i = 0; i <= 2; ++i) {
      stream >> token;
    }
  }
  return token;
}

long int System::UpTime() const {
  string token;
  for (string& line : SystemParser::Lines(Path::base + Path::uptime)) {
    std::istringstream stream(line);
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}

int System::TotalProcesses() const {
  string key, value;
  for (string& line : SystemParser::Lines(Path::base + Path::stat)) {
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
  for (string& line : SystemParser::Lines(Path::base + Path::stat)) {
    std::istringstream stream(line);
    while (stream >> key >> value) {
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}