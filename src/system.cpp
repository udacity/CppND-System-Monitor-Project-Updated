#include <cstddef>
#include <string>
#include <vector>

#include "system.h"
#include "system_parser.h"

using std::size_t;
using std::string;
using std::vector;

vector<string> System::IndividualCpuUtilizations() {
  vector<string> utilizations;
  vector<vector<string>> individual_cpu_times =
      SystemParser::IndividualCpuUtilizations();
  for (size_t i = 0; i < individual_cpu_times.size(); ++i) {
    utilizations.push_back("0");
    if (i < cached_individual_cpu_times_.size() &&
        cached_individual_cpu_times_[i].size() > 0) {
      utilizations[i] = SystemParser::CpuUtilization(
          cached_individual_cpu_times_[i], individual_cpu_times[i]);
    }
  }
  cached_individual_cpu_times_ = individual_cpu_times;
  return utilizations;
}

std::string System::AggregateCpuUtilization() {
  vector<string> cpu_times = SystemParser::AggregateCpuUtilization();
  string utilization{"0"};
  if (cached_aggregate_cpu_times_.size() > 0) {
    utilization =
        SystemParser::CpuUtilization(cached_aggregate_cpu_times_, cpu_times);
  }
  cached_aggregate_cpu_times_ = cpu_times;
  return utilization;
}

std::string System::MemoryUtilization() const {
  return std::to_string(SystemParser::MemoryUtilization());
}

long int System::UpTime() const { return SystemParser::UpTime(); }

std::string System::Kernel() const { return SystemParser::Kernel(); }

std::string System::TotalProcesses() const {
  return SystemParser::TotalProcesses();
}

std::string System::RunningProcesses() const {
  return SystemParser::RunningProcesses();
}

// TODO: Remove this unless it is used somewhere
std::string System::Threads() const { return ""; }

std::string System::OperatingSystem() const {
  return SystemParser::OperatingSystem();
}