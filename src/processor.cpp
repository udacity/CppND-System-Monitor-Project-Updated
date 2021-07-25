#include "processor.h"


float Processor::Utilization() { 

    auto nTotalJiffies = LinuxParser::Jiffies();
    auto nIdleJiffies = LinuxParser::IdleJiffies();

    auto changeTotal = nTotalJiffies - totalJiffies_;
    auto changeIdle = nIdleJiffies - idleJiffies_;

    totalJiffies_ = nTotalJiffies;
    idleJiffies_ = nIdleJiffies;

    return float(changeTotal - changeIdle) / float(changeTotal);
    
}