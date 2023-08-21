#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

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
long LinuxParser::Jiffies() 
{
  return ActiveJiffies() + IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) 
{
  std::string line;
  std::string line_element;
  long utime;
  long stime;
  long cutime;
  long cstime;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 13; ++i) {
      linestream >> line_element;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime + cutime + cstime;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  vector<long> cpu_util = CpuUtilization();
  return 
  (
    cpu_util[CPUStates::kUser_] + cpu_util[CPUStates::kNice_] + cpu_util[CPUStates::kSystem_] +
    cpu_util[CPUStates::kIRQ_] + cpu_util[CPUStates::kSoftIRQ_] + cpu_util[CPUStates::kSteal_]
  );
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  vector<long> cpu_util = CpuUtilization();
  return 
  (
    cpu_util[CPUStates::kIdle_] + cpu_util[CPUStates::kIOwait_]
  );
}

// Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization()
{
  vector<long> cpu_utilization;
  long cpu_util_element;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> cpu_util_element) {
      cpu_utilization.push_back(cpu_util_element);
    }

  }
  return cpu_utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string const kProcesses = "processes";
  return getKeyValue<int>(kProcesses, kStatFilename);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string const kRunningProcesses = "procs_running";
  return getKeyValue<int>(kRunningProcesses, kStatFilename);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid)
{
  string file = kProcDirectory + to_string(pid) + kCmdlineFilename;
  return getSingleValueFromFile<string>(file);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  int ram;
  string key = "VmRSS:";
  string file = "/" + to_string(pid) + kStatusFilename;
  ram = getKeyValue<int>(key, file);
  return to_string(ram / 1000);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{ 
  string key = "Uid:";
  string file = kProcDirectory +to_string(pid) + kStatusFilename;
  return getKeyValue<string>(key, file);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid)
{ 
  string uid = Uid(pid);
  string line, user, x, user_id;
  
  std::ifstream stream(kProcDirectory +to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> user_id;
      if (user_id == uid)
      {
        break;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid)
{
  long int start_time;
  string line_element;
  string line;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++)
    {
      linestream >> line;
    }
    // start_time is in 22nd element
    linestream >> start_time;
  }
  long int uptime = UpTime();
  return uptime - (start_time / sysconf(_SC_CLK_TCK));
}
