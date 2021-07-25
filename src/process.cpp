#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), upTime_(LinuxParser::UpTime(pid)), activeTime_(LinuxParser::ActiveJiffies(pid)) {

}

int Process::Pid() { return pid_; }

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

bool Process::operator<(Process const& a) const { return a.cpuUsage_ < this->cpuUsage_; }