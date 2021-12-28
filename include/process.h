#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_);
  int Pid();  // TODO: See src/process.cpp
  std::string Uid();
  std::string User();      // TODO: See src/process.cpp
  std::string Command();   // TODO: See src/process.cpp
  float CpuUtilization();  // TODO: See src/process.cpp
  long ActiveJiffies(int pid);
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  int pid;
  float last_cpu;
};

#endif