#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(std::string pid);
  std::string Pid() const;
  std::string User() const;
  std::string Cmd() const;
  std::string CpuUtilization() const;
  std::string Ram() const;
  std::string UpTime() const;
  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;

 private:
  std::string pid_;
  std::string user_;
  std::string cmd_;
  std::string cpu_;
  std::string ram_;
  std::string up_time_;
};

#endif