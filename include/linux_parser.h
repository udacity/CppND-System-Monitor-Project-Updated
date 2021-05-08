#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
std::unordered_set<int> Pids();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
struct CpuJiffies {
  std::string cpuID;
  long activeJiffies;
  long totalJiffies;
};

struct PidJiffies {
  long activeJiffies;
  long startTime;
};

std::vector<std::string> CpuUtilization();
std::vector<CpuJiffies> AllJiffies();

// Processes
PidJiffies ProcessJiffies(const int pid);
std::unordered_map<int, std::string> Usernames();
std::string Command(const int pid);
int Ram(const int pid);
int Uid(const int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif
