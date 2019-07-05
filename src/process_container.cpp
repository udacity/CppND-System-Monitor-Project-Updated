#include "process_container.h"
#include <string>
#include <vector>

ProcessContainer::ProcessContainer() {}

// TODO: Move into Process
void ProcessContainer::Refresh() {
  std::vector<std::string> pidList{ProcessParser::Pids()};
  processes_.clear();
  for (auto& pid : pidList) {
    processes_.emplace_back(pid);
  }
}

// std::string ProcessContainer::printList() {
//   std::string result = "";
//   for (int i = 0; i < this->processes_.size(); i++) {
//     result += this->processes_[i].getProcess();
//   }
//   return result;
// }

std::vector<Process> ProcessContainer::Processes() const { return processes_; }