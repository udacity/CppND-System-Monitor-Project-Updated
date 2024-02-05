#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

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

// DONE: Read and return the system memory utilization
// Memory Utilization = (Total Memory - Available Memory) / Total Memory
// Available Memory = Free Memory + Buffers + Cached
float LinuxParser::MemoryUtilization() {
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kProcDirectory + kMeminfoFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  float MemUtilization;
  float MemAvailable;
  std::vector<float> memInfo;
  while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    iss >> currentKey >> currentValue;
    if (currentKey == "MemTotal:") {
      // convert from kb to mb
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "MemFree:") {
      // convert from kb to mb
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "Buffers:") {
      // convert from kb to mb
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "Cached:") {
      // convert from kb to mb
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
  }
  MemAvailable = memInfo[1] + memInfo[2] + memInfo[3];
  MemUtilization = (((memInfo[0]) - (MemAvailable)) / (memInfo[0]) ) * 100;

  return MemUtilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kProcDirectory + kUptimeFilename
              << std::endl;
  }
  long uptime;
  while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    std::string currentValue;
    iss >> currentValue;
    uptime = std::stoi(currentValue);
    break;
  }  
  return uptime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kProcDirectory + kStatFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  int totalProcesses;
while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    iss >> currentKey >> currentValue;
    if (currentKey == "processes") {
      totalProcesses = std::stoi(currentValue);
    }
    break;
  }

  return totalProcesses; }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kProcDirectory + kStatFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  int runningProcesses;
while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    iss >> currentKey >> currentValue;
    if (currentKey == "procs_running") {
      runningProcesses = std::stoi(currentValue);
    }
    break;
  }

  return runningProcesses; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string id, x, temp, line;
  string name = "DEFAULT";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> temp >> x >> id;
      if (id == uid) {
        name = temp;
        break;
      }
    }
  }
  return name;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string line, value;
  vector<string> values;
  long starttime = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  try {
    starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    starttime = 0;
  }
  return starttime;
 }
