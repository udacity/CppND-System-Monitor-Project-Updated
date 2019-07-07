#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
std::vector<std::string> AggregateCpuUtilization();
std::vector<std::vector<std::string>> IndividualCpuUtilizations();
float CpuUtilization(std::vector<std::string> time1,
                     std::vector<std::string> time2);
float MemoryUtilization();
std::vector<std::string> Lines(std::string filepath);
long Active(std::vector<std::string> cpu);
long Idle(std::vector<std::string> cpu);
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
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kPasswordPath{"/etc/passwd"};
};  // namespace LinuxParser

#endif