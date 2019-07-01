#ifndef PROCESS_CONTAINER_H
#define PROCESS_CONTAINER_H

#include <vector>
#include "process.h"
#include "util.h"

class ProcessContainer {
 public:
  ProcessContainer();
  void Refresh();
  std::string printList();
  std::vector<Process> Processes() const;

 private:
  std::vector<Process> processes_;
};

#endif