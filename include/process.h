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
  std::string Command() const;
  float CpuUtilization() const;
  void CpuUtilization(float cpu);
  long Jiffies() const;
  std::string Ram() const;
  std::string UpTime() const;
  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;

 private:
  std::string pid_;
  float cpu_{0};
};

#endif