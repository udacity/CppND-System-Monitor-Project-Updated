#include "process_container.h"
#include <string>
#include <vector>

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
}

std::vector<Process> ProcessContainer::Processes() const { return processes_; }