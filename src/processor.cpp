#include "processor.h"

#include <vector>

// Return the aggregate CPU utilization
float Processor::Utilization() {
  const LinuxParser::systemCPUStat currUtil = LinuxParser::CpuUtilization();

  // The following calculations are taken from an answer in:
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  const long PrevIdle = prevUtil.idle_ + prevUtil.ioWait_;
  const long Idle = currUtil.idle_ + currUtil.ioWait_;

  const long PrevNonIdle = prevUtil.user_ + prevUtil.nice_ + prevUtil.system_ +
                     prevUtil.irq_ + prevUtil.softIRQ_ + prevUtil.steal_;
  const long NonIdle = currUtil.user_ + currUtil.nice_ + currUtil.system_ +
                 currUtil.irq_ + currUtil.softIRQ_ + currUtil.steal_;

  const long PrevTotal = PrevIdle + PrevNonIdle;
  const long Total = Idle + NonIdle;

  const long totald = Total - PrevTotal;
  const long idled = Idle - PrevIdle;

  const float CPU_Percentage = (totald - idled) * 1.00 / totald;

  // For the next calculation, currUtil becomes prevUtil
  prevUtil = currUtil;

  return CPU_Percentage;
}