#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include <linux_parser.h>


class Processor {
 public:
  Processor() : idleJiffies_(LinuxParser::IdleJiffies()), totalJiffies_(LinuxParser::Jiffies()) {}
  float Utilization();  

 private:
    long idleJiffies_;
    long totalJiffies_;
    
};

#endif