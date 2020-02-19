#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    LinuxParser::CPUStat currUtil = LinuxParser::CpuUtilization();
    
    // The following calculations are taken from an answer in:
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    long PrevIdle = prevUtil.idle_ + prevUtil.ioWait_;
    long Idle = currUtil.idle_ + currUtil.ioWait_;

    long PrevNonIdle = prevUtil.user_ + prevUtil.nice_
                        + prevUtil.system_ + prevUtil.irq_
                        + prevUtil.softIRQ_ + prevUtil.steal_;
    long NonIdle = currUtil.user_ + currUtil.nice_
                    + currUtil.system_ + currUtil.irq_
                    + currUtil.softIRQ_ + currUtil.steal_;

    long PrevTotal = PrevIdle + PrevNonIdle;
    long Total = Idle + NonIdle;

    long totald = Total - PrevTotal;
    long idled = Idle - PrevIdle;

    float CPU_Percentage = (totald - idled)*1.00/totald;

    // For the next calculation, currUtil becomes prevUtil
    prevUtil = currUtil;

    return CPU_Percentage;
}