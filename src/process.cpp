#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }
void Process::Pid(int p) { pid_ = p; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return command_;}
void Process::Command(string c) { command_ = c; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }
void Process::Ram(string r) { ram_ = r; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }
void Process::User(string u) { user_ = u; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }
void Process::UpTime(long u) { uptime_ = u; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }