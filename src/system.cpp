#include "system.h"
#include "system_parser.h"

using std::size_t;

System::System() {
  /*
  Getting initial info about system
  Initial data for individual cores is set
  System data is set
  */
  // this->getOtherCores(SystemParser::CpuCores());
  // this->setLastCpuMeasures();
  // this->setAttributes();
  // this->os_name_ = ProcessParser::getOSName();
  // this->kernel_version_ = ProcessParser::getSysKernelVersion();
}

// void System::getOtherCores(int size) {
//   // when number of cores is detected, vectors are modified to fit incoming
//   data this->coresStats = std::vector<std::string>();
//   this->coresStats.resize(size);
//   this->lastCpuCoresStats = std::vector<std::vector<std::string>>();
//   this->lastCpuCoresStats.resize(size);
//   this->currentCpuCoresStats = std::vector<std::vector<std::string>>();
//   this->currentCpuCoresStats.resize(size);
//   for (int i = 0; i < size; i++) {
//     this->lastCpuCoresStats[i] =
//         ProcessParser::getSysCpuPercent(std::to_string(i));
//   }
// // }

// void System::setLastCpuMeasures() {
//   this->lastCpuStats = ProcessParser::getSysCpuPercent("");
// }

// void System::setCpuCoresStats() {
//   // Getting data from files (previous data is required)
//   for (size_t i = 0; i < this->currentCpuCoresStats.size(); i++) {
//     this->currentCpuCoresStats[i] =
//         ProcessParser::getSysCpuPercent(std::to_string(i));
//   }
//   for (size_t i = 0; i < this->currentCpuCoresStats.size(); i++) {
//     // after acquirement of data we are calculating every core percentage of
//     // usage
//     this->coresStats[i] = ProcessParser::PrintCpuStats(
//         this->lastCpuCoresStats[i], this->currentCpuCoresStats[i]);
//   }
//   this->lastCpuCoresStats = this->currentCpuCoresStats;
// }

void System::Refresh() {
  // getting parsed data
  // this->memPercent = ProcessParser::getSysRamPercent();
  // this->up_time_ = ProcessParser::getSysUpTime();
  // this->total_procs_ = ProcessParser::getTotalNumberOfProcesses();
  // this->running_proc_ = ProcessParser::getNumberOfRunningProcesses();
  // this->threads_ = ProcessParser::getTotalThreads();
  // this->currentCpuStats = ProcessParser::getSysCpuPercent("");
  // this->cpu_ =
  //     ProcessParser::PrintCpuStats(this->lastCpuStats,
  //     this->currentCpuStats);
  // this->lastCpuStats = this->currentCpuStats;
  // this->setCpuCoresStats();
}

// Constructing string for every core data display
// std::vector<std::string> System::getCoresStats() const {
//   std::vector<std::string> result = std::vector<std::string>();
//   for (size_t i = 0; i < this->coresStats.size(); i++) {
//     std::string temp = ("cpu" + std::to_string(i) + ": ");
//     float check{0};
//     if (!this->coresStats[i].empty()) check = stof(this->coresStats[i]);
//     if (!check || this->coresStats[i] == "nan") {
//       return std::vector<std::string>();
//     }
//     temp += Util::GetProgressBar(this->coresStats[i]);
//     result.push_back(temp);
//   }
//   return result;
// }

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