#include "process_container.h"
#include <string>
#include <vector>

ProcessContainer::ProcessContainer() { this->refreshList(); }

void ProcessContainer::refreshList() {
  std::vector<std::string> pidList = ProcessParser::getPidList();
  this->list_.clear();
  for (int i = 0; i < pidList.size(); i++) {
    Process proc(pidList[i]);
    this->list_.push_back(proc);
  }
}

std::string ProcessContainer::printList() {
  std::string result = "";
  for (int i = 0; i < this->list_.size(); i++) {
    result += this->list_[i].getProcess();
  }
  return result;
}

std::vector<std::vector<std::string>> ProcessContainer::getList() {
  std::vector<std::vector<std::string>> values;
  std::vector<std::string> stringifiedList;
  for (int i = 0; i < ProcessContainer::list_.size(); i++) {
    stringifiedList.push_back(ProcessContainer::list_[i].getProcess());
  }
  int lastIndex = 0;
  for (int i = 0; i < stringifiedList.size(); i++) {
    if (i % 10 == 0 && i > 0) {
      std::vector<std::string> sub(&stringifiedList[i - 10],
                                   &stringifiedList[i]);
      values.push_back(sub);
      lastIndex = i;
    }
    if (i == (ProcessContainer::list_.size() - 1) && (i - lastIndex) < 10) {
      std::vector<std::string> sub(&stringifiedList[lastIndex],
                                   &stringifiedList[i + 1]);
      values.push_back(sub);
    }
  }
  return values;
}