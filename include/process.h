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
  std::string User() const;
  std::string Cmd() const;
  std::string CpuUtilization() const;
  std::string Ram() const;
  std::string UpTime() const;
  void Update();

 private:
  std::string pid_;
  std::string user_;
  std::string cmd_;
  std::string cpu_;
  std::string ram_;
  std::string up_time_;
};

#endif