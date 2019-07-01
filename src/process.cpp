#include "process.h"
#include <string>

Process::Process(std::string pid) : pid_(pid) {
  // this->pid = pid;
  // this->user = ProcessParser::getProcUser(pid);
  // TODOs:
  // complete for mem
  // complete for cmd
  // complete for upTime
  // complete for cpu
}

void Process::Pid(int pid) { this->pid_ = pid; }

std::string Process::Pid() const { return this->pid_; }

std::string Process::getProcess() {
  if (!ProcessParser::isPidExisting(this->pid_)) return "";
  this->mem = ProcessParser::VmSize(this->pid_);
  this->upTime = ProcessParser::getProcUpTime(this->pid_);
  this->cpu = ProcessParser::getCpuPercent(this->pid_);

  return (this->pid_ + "   ");  // TODO: finish the string! this->user + "   "+
                               // mem...cpu...upTime...;
}