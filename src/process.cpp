#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//create constructor
Process::Process(int pid):
			  pid_(pid),
			  cmd_(LinuxParser::Command(pid)),
			  user_(LinuxParser::User(pid)),
			  ram_(LinuxParser::Ram(Pid()))
{};

// TODO: Return this process's ID
int Process::Pid() const{ return pid_; } //return pid_;

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
	float totaltime = LinuxParser::ActiveJiffies(Pid());  // In jiffies
	float uptime = LinuxParser::UpTime();                 // In seconds
	float secondsactive = uptime - (Process::UpTime() / sysconf(_SC_CLK_TCK));  // In seconds
	float cpu_usage = (totaltime / sysconf(_SC_CLK_TCK)) / secondsactive;  // In seconds
	return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return cmd_ ; }//

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const{ return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return (CpuUtilization() > a.CpuUtilization());
}
