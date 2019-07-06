#include "process_container.h"
#include <string>
#include <vector>

using std::stof;

// TODO: Move into Process
void ProcessContainer::Update() {
  std::vector<std::string> pidList{ProcessParser::Pids()};
  processes_.clear();
  for (auto& pid : pidList) {
    Process process(pid);
    process.Update();
    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end(), ProcessContainer::Compare);
}

std::vector<Process> ProcessContainer::Processes() const { return processes_; }

bool ProcessContainer::Compare(const Process& a, const Process& b) {
  return stof(a.CpuUtilization()) > stof(b.CpuUtilization());
}