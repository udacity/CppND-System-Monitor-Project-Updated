#ifndef PROCESS_CONTAINER_H
#define PROCESS_CONTAINER_H

#include <vector>
#include "process.h"
#include "util.h"

class ProcessContainer {
 public:
  ProcessContainer();
  void refreshList();
  std::string printList();
  std::vector<std::vector<std::string>> getList();

 private:
  std::vector<Process> list_;
};

#endif