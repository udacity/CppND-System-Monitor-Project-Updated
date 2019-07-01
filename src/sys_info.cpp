#include "sys_info.h"

SysInfo::SysInfo() {
  /*
  Getting initial info about system
  Initial data for individual cores is set
  System data is set
  */
  this->getOtherCores(ProcessParser::getNumberOfCores());
  this->setLastCpuMeasures();
  this->setAttributes();
  this->os_name_ = ProcessParser::getOSName();
  this->kernel_version_ = ProcessParser::getSysKernelVersion();
}

void SysInfo::getOtherCores(int size) {
  // when number of cores is detected, vectors are modified to fit incoming data
  this->coresStats = std::vector<std::string>();
  this->coresStats.resize(size);
  this->lastCpuCoresStats = std::vector<std::vector<std::string>>();
  this->lastCpuCoresStats.resize(size);
  this->currentCpuCoresStats = std::vector<std::vector<std::string>>();
  this->currentCpuCoresStats.resize(size);
  for (int i = 0; i < size; i++) {
    this->lastCpuCoresStats[i] =
        ProcessParser::getSysCpuPercent(std::to_string(i));
  }
}

void SysInfo::setLastCpuMeasures() {
  this->lastCpuStats = ProcessParser::getSysCpuPercent("");
}

void SysInfo::setCpuCoresStats() {
  // Getting data from files (previous data is required)
  for (int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    this->currentCpuCoresStats[i] =
        ProcessParser::getSysCpuPercent(std::to_string(i));
  }
  for (int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    // after acquirement of data we are calculating every core percentage of
    // usage
    this->coresStats[i] = ProcessParser::PrintCpuStats(
        this->lastCpuCoresStats[i], this->currentCpuCoresStats[i]);
  }
  this->lastCpuCoresStats = this->currentCpuCoresStats;
}

void SysInfo::setAttributes() {
  // getting parsed data
  this->memPercent = ProcessParser::getSysRamPercent();
  this->up_time_ = ProcessParser::getSysUpTime();
  this->total_procs_ = ProcessParser::getTotalNumberOfProcesses();
  this->running_proc_ = ProcessParser::getNumberOfRunningProcesses();
  this->threads_ = ProcessParser::getTotalThreads();
  this->currentCpuStats = ProcessParser::getSysCpuPercent("");
  this->cpuPercent =
      ProcessParser::PrintCpuStats(this->lastCpuStats, this->currentCpuStats);
  this->lastCpuStats = this->currentCpuStats;
  this->setCpuCoresStats();
}

// Constructing string for every core data display
std::vector<std::string> SysInfo::getCoresStats() const {
  std::vector<std::string> result = std::vector<std::string>();
  for (int i = 0; i < this->coresStats.size(); i++) {
    std::string temp = ("cpu" + std::to_string(i) + ": ");
    float check;
    if (!this->coresStats[i].empty()) check = stof(this->coresStats[i]);
    if (!check || this->coresStats[i] == "nan") {
      return std::vector<std::string>();
    }
    temp += Util::GetProgressBar(this->coresStats[i]);
    result.push_back(temp);
  }
  return result;
}

std::string SysInfo::getCpuPercent() const { return this->cpuPercent; }

std::string SysInfo::getMemPercent() const {
  return std::to_string(this->memPercent);
}

long SysInfo::getUpTime() const { return this->up_time_; }

std::string SysInfo::getKernelVersion() const { return this->kernel_version_; }

std::string SysInfo::getTotalProc() const {
  return std::to_string(this->running_proc_);
}

std::string SysInfo::getRunningProc() const {
  return std::to_string(this->running_proc_);
}

std::string SysInfo::getThreads() const {
  return std::to_string(this->threads_);
}

std::string SysInfo::getOSName() const { return this->os_name_; }