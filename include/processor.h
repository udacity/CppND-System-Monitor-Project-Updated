#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  Processor(std::string cpuID, float active, float total) : cpuID_(cpuID),
    active_(active), total_(total) {}
  std::string CpuID() const { return cpuID_; }
  float Utilization() const;  // See src/processor.cpp
  void UpdateUtilization(long active, long total); // See src/processor.cpp

  // Declare any necessary private members
 private:
  std::string cpuID_;
  long prevActive_ = 0L;
  long prevTotal_ = 0L;
  long active_ = 0L;
  long total_ = 0L;
};

#endif
