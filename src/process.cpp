#include "process.h"
#include <string>

Process::Process(std::string pid) {
  this->pid = pid;
  this->user = ProcessParser::getProcUser(pid);
  // TODOs:
  // complete for mem
  // complete for cmd
  // complete for upTime
  // complete for cpu
}

void Process::setPid(int pid) { this->pid = pid; }

std::string Process::getPid() const { return this->pid; }

std::string Process::getProcess() {
  if (!ProcessParser::isPidExisting(this->pid)) return "";
  this->mem = ProcessParser::getVmSize(this->pid);
  this->upTime = ProcessParser::getProcUpTime(this->pid);
  this->cpu = ProcessParser::getCpuPercent(this->pid);

  return (this->pid + "   ");  // TODO: finish the string! this->user + "   "+
                               // mem...cpu...upTime...;
}