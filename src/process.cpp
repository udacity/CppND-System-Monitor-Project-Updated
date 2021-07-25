#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Comple Process contructor
Process::Process(int pid) : pid_(pid), upTime_(LinuxParser::UpTime(pid)), activeTime_(LinuxParser::ActiveJiffies(pid)) {

}

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    auto nUpTime = LinuxParser::UpTime();
    auto nActiveTime = LinuxParser::ActiveJiffies(pid_);

    auto changeUpTime = nUpTime - upTime_;
    auto changeActiveTime = nActiveTime - activeTime_;

    upTime_ = nUpTime;
    activeTime_ = nActiveTime;

    return float(changeActiveTime / sysconf(_SC_CLK_TCK)) / float(changeUpTime);

}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }