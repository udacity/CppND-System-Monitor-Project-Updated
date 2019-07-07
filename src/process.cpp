#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpu_; }

void Process::CpuUtilization(long active_ticks, long system_ticks) {
  long duration_active{active_ticks - cached_active_ticks_};
  long duration{system_ticks - cached_system_ticks_};
  cpu_ = static_cast<float>(duration_active) / duration;
  cached_active_ticks_ = active_ticks;
  cached_system_ticks_ = system_ticks;
}

string Process::Command() const { return LinuxParser::Command(Pid()); }

string Process::Ram() const { return LinuxParser::Ram(Pid()); }

string Process::User() const { return LinuxParser::User(Pid()); }

long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}