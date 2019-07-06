#include "process_container.h"
#include <string>
#include <vector>

using std::stof;

ProcessContainer::ProcessContainer() {}

// TODO: Move into Process
void ProcessContainer::Update() {
  std::vector<std::string> pidList{ProcessParser::Pids()};
  processes_.clear();
  for (auto& pid : pidList) {
    processes_.emplace_back(pid);
  }
  for (auto& process : processes_) {
    process.Update();
  }
  std::sort(processes_.begin(), processes_.end(), ProcessContainer::Compare);
}

std::vector<Process> ProcessContainer::Processes() const { return processes_; }

bool ProcessContainer::Compare(const Process& a, const Process& b) {
  return stof(a.CpuUtilization()) > stof(b.CpuUtilization());
}