#include "processor.h"

Processor::Processor() {
    refreshData(LinuxParser::CpuUtilization());
}

void Processor::refreshData(std::vector<unsigned int> cpuData) {
        cpuValues = cpuData;
}



float Processor::Utilization() { 
    auto nCpuData = LinuxParser::CpuUtilization();

    //Substracting virtual time
    nCpuData[PROC_TIME_USER] -= nCpuData[PROC_TIME_GUEST];
    nCpuData[PROC_TIME_NICE] -= nCpuData[PROC_TIME_GUEST_NICE];
    cpuValues[PROC_TIME_USER] -= cpuValues[PROC_TIME_GUEST]; 
    cpuValues[PROC_TIME_NICE] -= cpuValues[PROC_TIME_GUEST_NICE]; 

    unsigned int Total = 0;
    unsigned int prevTotal = 0;
    for(auto value : nCpuData) Total += value;
    for(auto pvalue : cpuValues) prevTotal += pvalue;

    auto Idle = nCpuData[PROC_TIME_IDLE] + nCpuData[PROC_TIME_IOWAIT];
    auto prevIdle = cpuValues[PROC_TIME_IDLE] + cpuValues[PROC_TIME_IOWAIT];

    // auto NonIdle = Total - Idle;
    // auto prevNonIdle = prevTotal - prevIdle;

    auto changeTotal = Total - prevTotal;
    auto changeIdle = Idle - prevIdle;

    refreshData(nCpuData);

    return float(changeTotal - changeIdle) / float(changeTotal);
    
}