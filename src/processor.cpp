#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() const {
  long total_d = total_-prevTotal_;
  long active_d = active_-prevActive_;
  return (total_d ? active_d/(float)total_d : 0.0f);
}

void Processor::UpdateUtilization(long active, long total) {
  prevActive_ = active_;
  prevTotal_ = total_;
  active_ = active;
  total_ = total;
}
