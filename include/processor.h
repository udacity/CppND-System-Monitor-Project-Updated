#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // Return the aggregate CPU utilization

 private:
  LinuxParser::systemCPUStat prevUtil;
};

#endif