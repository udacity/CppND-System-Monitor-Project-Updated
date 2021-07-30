#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : 
pid_(pid), systemJiffies_(LinuxParser::Jiffies()), procJiffies_(LinuxParser::ActiveJiffies(pid)), upTime_(LinuxParser::UpTime(pid)) {

}

int Process::Pid() { return pid_; }

void Process::UpdateCPU() {
    auto nSystemJiffies = LinuxParser::Jiffies();
    auto nProcJiffies = LinuxParser::ActiveJiffies(pid_);

    auto changeTotal = nSystemJiffies - systemJiffies_;
    auto changeProc = nProcJiffies - procJiffies_; 

    systemJiffies_ = nSystemJiffies;
    procJiffies_ = nProcJiffies;

    cpuUsage_ = float(changeProc) / float(changeTotal);
}

float Process::CpuUtilization() { return cpuUsage_; }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime() - upTime_; }

bool Process::operator<(Process const& a) const { return a.cpuUsage_ < this->cpuUsage_; }

bool Process::operator==(Process const& a) const { return a.pid_ == this->pid_;}

bool Process::operator==(int a) const { return a == this->pid_;}