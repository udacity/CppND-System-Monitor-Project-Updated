#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  std::string const kMemTotalKey = "MemTotal:";
  std::string const kMemAvailKey = "MemAvailable:";
  float mem_total = getKeyValue<float>(kMemTotalKey, kMeminfoFilename);
  float mem_avail = getKeyValue<float>(kMemAvailKey, kMeminfoFilename);

  return (mem_total - mem_avail) / mem_total; 
}

// Read and return the system uptime
long LinuxParser::UpTime()
{ 
  return getSingleValueFromFile<long>(kUptimeFilename); 
}

// Read and return the number of jiffies for the system
float LinuxParser::Jiffies() 
{
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) 
{
  return 0;
}

// Read and return the number of active jiffies for the system
float LinuxParser::ActiveJiffies() 
{
  vector<float> cpu_util = CpuUtilization();
  return 
  (
    cpu_util[CPUStates::kUser_] + cpu_util[CPUStates::kNice_] + cpu_util[CPUStates::kSystem_] +
    cpu_util[CPUStates::kIRQ_] + cpu_util[CPUStates::kSoftIRQ_] + cpu_util[CPUStates::kSteal_]
  );
}

// Read and return the number of idle jiffies for the system
float LinuxParser::IdleJiffies() 
{
  vector<float> cpu_util = CpuUtilization();
  return 
  (
    cpu_util[CPUStates::kIdle_] + cpu_util[CPUStates::kIOwait_]
  );
}

// Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization() 
{
  vector<float> cpu_util_columns;
  float cpu_util_column;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> cpu_util_column)
    {
      cpu_util_columns.push_back(cpu_util_column);
    }

  }
  return cpu_util_columns;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  std::string const kProcesses = "processes";
  return getKeyValue<int>(kProcesses, kStatFilename);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::string const kRunningProcesses = "procs_running";
  return getKeyValue<int>(kRunningProcesses, kStatFilename);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
