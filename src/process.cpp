#include "process.h"
#include <cctype>
#include <string>
#include "process_parser.h"

using std::string;

Process::Process(string pid) : pid_(pid) {
  command_ = ProcessParser::Cmdline(pid_);
  cpu_ = ProcessParser::CpuUtilization(pid_);
  ram_ = ProcessParser::VmSize(pid_);
  up_time_ = ProcessParser::UpTime(pid_);
  user_ = ProcessParser::User(pid_);
}

string Process::Command() const { return command_; }

string Process::CpuUtilization() const { return cpu_; }

string Process::Pid() const { return pid_; }

string Process::Ram() const { return ram_; }

string Process::User() const { return user_; }

string Process::UpTime() const { return up_time_; }

bool Process::operator<(const Process& a) const {
  return stof(cpu_) < stof(a.cpu_);
}

bool Process::operator>(const Process& a) const {
  return stof(cpu_) > stof(a.cpu_);
}