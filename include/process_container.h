#ifndef PROCESS_CONTAINER_H
#define PROCESS_CONTAINER_H

#include <vector>
#include "process.h"
#include "util.h"

// TODO: Move into System
class ProcessContainer {
 public:
  ProcessContainer();
  void Update();
  std::string printList();
  std::vector<Process> Processes() const;

 private:
  std::vector<Process> processes_;
  static bool Compare(const Process& a, const Process& b);
};

#endif