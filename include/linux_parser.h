#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <iostream>

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
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

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
std::vector<float> CpuUtilization();
float Jiffies();
float ActiveJiffies();
long ActiveJiffies(int pid);
float IdleJiffies();

// helper functions
template <typename T>
T getKeyValue(std::string const &keyIn, std::string const &file)
{
  std::string line, key;
  T value;
  std::ifstream stream(kProcDirectory + file);
  if (stream.is_open()) 
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) 
      {
        if (key == keyIn) 
        {
          return value;
        }
      }
    }

  }
  //std::cout << "getKeyValue -- end of file reached without finding: " << keyIn << std::endl;
  return value;
}

template <typename T>
T getSingleValueFromFile(std::string const &file)
{
  std::string line;
  T value;
  std::ifstream stream(kProcDirectory + file);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  //std::cout << "getSingleValueFromFile -- value: " << value << std::endl;
  return value;
}

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif