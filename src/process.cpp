#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

void Process::UpdateStats(long time_used, long time_elapsed, int vm_usage) {
    prev_time_used_ = time_used_;
    time_used_ = time_used;
    prev_time_elapsed_ = time_elapsed_;
    time_elapsed_ = time_elapsed;
    vm_usage_ = vm_usage;
    cpu_util_ = Process::ComputeAvgCpuUtil();
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_util_;}

float Process::ComputeCpuUtil() const {
  long total_d = time_elapsed_-prev_time_elapsed_;
  long active_d = time_used_-prev_time_used_;
  return (total_d ? active_d/(float)total_d : 0.0f);
}

float Process::ComputeAvgCpuUtil() const {
  return (time_elapsed_> 0 ? time_used_/(float)time_elapsed_ : 0.0f);
}

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
int Process::Ram() const { return vm_usage_; }

// Return the user (name) that generated this process
string Process::User() const { return user_name_; }

// Return the age of this process (in seconds)
long Process::UpTime(long sys_uptime) const { return sys_uptime-start_time_; }

// Return start time of this process for sorting by uptime
long Process::StartTime() const { return start_time_; }
