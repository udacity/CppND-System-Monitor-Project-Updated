#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);        // Constructor instantiates all private variables
  int Pid();               // Return this process's ID
  std::string User();      // Return the user (name) that generated this process
  std::string Command();   // Return the command that generated this process
  float CpuUtilization();  // Return this process's CPU utilization
  std::string Ram();       // Return this process's memory utilization
  long int UpTime();       // Return the age of this process (in seconds)

  // Overload the "less than" comparison operator for Process objects
  bool operator<(Process const& a) const;

 private:
  float getCPUUtilization();  // Helper function to get the cpu utilization for
                              // this process

  int pid_;
  std::string user_;
  std::string command_;
  std::string ram_;
  long uptime_;
  float cpu_;
};

#endif