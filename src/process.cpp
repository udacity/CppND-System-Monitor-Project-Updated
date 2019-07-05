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

string Process::Pid() const { return this->pid_; }

string Process::Ram() const { return ram_; }

string Process::Refresh() {
  if (!ProcessParser::isPidExisting(this->pid_)) return "";
  this->ram_ = ProcessParser::VmSize(this->pid_);
  this->upTime = ProcessParser::getProcUpTime(this->pid_);
  this->cpu = ProcessParser::getCpuPercent(this->pid_);

  return (this->pid_ + "   ");  // TODO: finish the string! this->user + "   "+
                                // mem...cpu...upTime...;
}