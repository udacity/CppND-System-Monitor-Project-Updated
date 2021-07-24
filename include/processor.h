#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include <linux_parser.h>

enum proc_time_t {
    PROC_TIME_USER,
    PROC_TIME_NICE,
    PROC_TIME_SYSTEM,
    PROC_TIME_IDLE,
    PROC_TIME_IOWAIT,
    PROC_TIME_IRQ,
    PROC_TIME_SOFTIRQ,
    PROC_TIME_STEAL,
    PROC_TIME_GUEST,
    PROC_TIME_GUEST_NICE
};

class Processor {
 public:
  Processor();
  void refreshData(std::vector<unsigned int> cpuData);
  float Utilization();  

 private:
    std::vector<unsigned int> cpuValues = {};
    
};

#endif