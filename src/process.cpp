#include "process.h"
#include <string>

using std::string;

Process::Process(string pid) : pid_(pid) {
  // TODOs:
  // complete for cmd
  // complete for upTime
  // complete for cpu
}

void Process::Pid(int pid) { this->pid_ = pid; }

string Process::Cmd() const { return cmd_; }

string Process::Cpu() const { return cpu_; }

string Process::Pid() const { return pid_; }

string Process::Ram() const { return ram_; }

string Process::User() const { return user_; }

string Process::UpTime() const { return up_time_; }

void Process::Refresh() {
  if (ProcessParser::isPidExisting(this->pid_)) {
    cmd_ = ProcessParser::Cmd(pid_);
    cpu_ = ProcessParser::Cpu(pid_);
    ram_ = ProcessParser::VmSize(pid_);
    up_time_ = ProcessParser::UpTime(pid_);
    user_ = ProcessParser::User(pid_);
  }
}