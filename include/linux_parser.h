#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// ---------Paths----------
const std::string kProcDirectory{"/proc"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// --------System--------
// Read and return system's memory utilization
float MemoryUtilization();

// Read and return the total number os seconds system has been On for
long UpTime();

// Read directory names and return - these are the process pid's
std::vector<int> Pids();

// Read and return the total number of processes
int TotalProcesses();

// Read and return the total number of processes that are running
int RunningProcesses();

// Read and return the OS name
std::string OperatingSystem();

// Read and return kernel
std::string Kernel();

// -----------CPU------------
// Helper struct to store the cpu usage details
struct systemCPUStat {
 public:
  systemCPUStat()
      : user_(0),
        nice_(0),
        system_(0),
        idle_(0),
        ioWait_(0),
        irq_(0),
        softIRQ_(0),
        steal_(0) {}
  long user_;
  long nice_;
  long system_;
  long idle_;
  long ioWait_;
  long irq_;
  long softIRQ_;
  long steal_;
};
// Read and return the details necessary to calculate system's CPU utilization
systemCPUStat CpuUtilization();

// ----------Processes-----------
// Helper struct to store the cpu usage details for process
struct procCPUStat {
 public:
  procCPUStat() : utime_(0), stime_(0), cutime_(0), cstime_(0) {}
  long utime_;
  long stime_;
  long cutime_;
  long cstime_;
};
// Read and return the details necessary to calculate a process's CPU
// utilization
procCPUStat CpuUtilization(int pid);

// Read and return the command associated with a process
std::string Command(int pid);

// Read and return the memory used by a process
std::string Ram(int pid);

// Read and return user ID associated with a process
int Uid(int pid);

// Read and return the user name for a given user ID
std::string User(int uid);

// Get the start time of process in seconds
long int UpTime(int pid);
};  // namespace LinuxParser

#endif