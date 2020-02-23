#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Constructor instantiates all private variables
Process::Process(int pid) : pid_(pid) {
  const int uid = LinuxParser::Uid(pid_);
  user_ = LinuxParser::User(uid);
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  uptime_ = LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
  cpu_ = getCPUUtilization();
}

// Helper function to get the cpu utilization for this process
// Calculated using the detaild from:
//    https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::getCPUUtilization() {
  LinuxParser::procCPUStat util = LinuxParser::CpuUtilization(pid_);
  long sysUptime = LinuxParser::UpTime();
  long total_time = util.utime_ + util.stime_ + util.cutime_ + util.cstime_;
  total_time /= sysconf(_SC_CLK_TCK);
  long seconds = sysUptime - uptime_;
  float res = 1.0 * total_time / seconds;
  return res;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return cpu_ > a.cpu_; }