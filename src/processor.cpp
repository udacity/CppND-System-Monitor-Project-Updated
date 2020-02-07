#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    long current_cpu_usage_active = LinuxParser::ActiveJiffies();
    long current_cpu_usage_idle = LinuxParser::IdleJiffies();
    long cpu_usage_active_delta = current_cpu_usage_active - previous_cpu_usage_active_; 
    long cpu_usage_idle_delta = current_cpu_usage_idle - previous_cpu_usage_idle_; 
    previous_cpu_usage_active_ = current_cpu_usage_active;
    previous_cpu_usage_idle_ = current_cpu_usage_idle;

    return cpu_usage_active_delta/(cpu_usage_idle_delta + cpu_usage_active_delta) * 1.0; 
}