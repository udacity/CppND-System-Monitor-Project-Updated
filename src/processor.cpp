#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    float total_jiffies = LinuxParser::Jiffies();
    float active_jiffies = LinuxParser::ActiveJiffies();
    float total_delta = total_jiffies - prev_total_jiffies_;
    float active_delta = active_jiffies - prev_active_jiffies_;
    prev_total_jiffies_ = total_jiffies;
    prev_active_jiffies_ = active_jiffies;
    return active_delta / total_delta;
}