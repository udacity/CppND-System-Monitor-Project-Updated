#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<long> currUtil = LinuxParser::CpuUtilization();
    //_0____|_1____|_2______|_3____|_4______|_5___|_6_______|_7____
    // user | nice | system | idle | iowait | irq | softirq | steal

    // The following calculations are taken from an answer in:
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    long PrevIdle = prevUtil[3] + prevUtil[4];
    long Idle = currUtil[3] + currUtil[4];

    long PrevNonIdle = prevUtil[0] + prevUtil[1] + prevUtil[2] + prevUtil[5] + prevUtil[6] + prevUtil[7];
    long NonIdle = currUtil[0] + currUtil[1] + currUtil[2] + currUtil[5] + currUtil[6] + currUtil[7];

    long PrevTotal = PrevIdle + PrevNonIdle;
    long Total = Idle + NonIdle;

    long totald = Total - PrevTotal;
    long idled = Idle - PrevIdle;

    float CPU_Percentage = (totald - idled)*1.00/totald;

    // For the next calculation, currUtil becomes prevUtil
    prevUtil = currUtil;

    return CPU_Percentage;
}