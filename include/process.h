#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "process_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(std::string pid);
  void Pid(int pid);
  std::string Pid() const;
  std::string getUser() const;
  std::string getCmd() const;
  int getCpu() const;
  int getMem() const;
  std::string getUpTime() const;
  std::string getProcess();

 private:
  std::string pid_;
  std::string user;
  std::string cmd;
  std::string cpu;
  std::string mem;
  std::string upTime;
};

#endif