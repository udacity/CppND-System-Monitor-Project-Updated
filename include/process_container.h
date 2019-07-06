#ifndef PROCESS_CONTAINER_H
#define PROCESS_CONTAINER_H

#include <vector>
#include "process.h"
#include "util.h"

// TODO: Move into System
class ProcessContainer {
 public:
  void Update();
  std::vector<Process> Processes() const;

 private:
  std::vector<Process> processes_;
  static bool Compare(const Process& a, const Process& b);
};

#endif