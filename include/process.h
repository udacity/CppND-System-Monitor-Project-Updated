#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  Process(int pid, std::string user_name, std::string cmd, long time_used,
        long time_elapsed, int vm_usage, long start_time) : 
        pid_(pid), user_name_(user_name), command_(cmd), start_time_(start_time), 
        time_used_(time_used), time_elapsed_(time_elapsed),
    prev_time_elapsed_(start_time), vm_usage_(vm_usage),cpu_util_(ComputeAvgCpuUtil()) {}

  void UpdateStats(long time_used, long time_elapsed, int vm_usage);

  int Pid() const;                         // See src/process.cpp
  std::string User() const;                // See src/process.cpp
  std::string Command() const;             // See src/process.cpp
  float CpuUtilization() const;            // See src/process.cpp
  int Ram() const;                         // See src/process.cpp
  long UpTime(long sys_uptime) const;      // See src/process.cpp
  long StartTime() const;                  // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp

  // Declare any necessary private members
 private:
  int pid_ {0};
  std::string user_name_ {};
  std::string command_ {};
  long start_time_ {0L};
  long time_used_ {0L};
  long prev_time_used_ {0L};
  long time_elapsed_ {0L};
  long prev_time_elapsed_ {0L};
  int vm_usage_ {0};
  float cpu_util_ {0.0f};
  float ComputeCpuUtil() const;
  float ComputeAvgCpuUtil() const;
};

#endif
