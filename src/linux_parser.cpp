#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <cmath>

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
    iss >> currentKey >> currentValue;
    if (currentKey == "MemTotal:") {
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "MemFree:") {
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "Buffers:") {
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
    if (currentKey == "Cached:") {
      float val = stof(currentValue) / 1024 / 1024;
      memInfo.push_back(val);
    }
  }
  MemAvailable = memInfo[1] + memInfo[2] + memInfo[3];
  MemUtilization = (((memInfo[0]) - (MemAvailable)) / (memInfo[0])) * 100;

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
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kProcDirectory + kStatFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  vector<string> cpuUtilization;
  double cpuUtilization0;
  while (getline(filestream, line)) {
    // PrevIdle = previdle + previowait
    // Idle = idle + iowait

    // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
    // prevsteal NonIdle = user + nice + system + irq + softirq + steal

    // PrevTotal = PrevIdle + PrevNonIdle
    // Total = Idle + NonIdle

    // # differentiate: actual value minus the previous one
    // totald = Total - PrevTotal
    // idled = Idle - PrevIdle

    // CPU_Percentage = (totald - idled)/totald

    std::istringstream iss(line);
    iss >> currentKey;
    if (currentKey == "cpu") {
      int user, nice, system, idle, iowait, irq, softirq, steal, guest,
          guest_nice;

      while (iss >> user >> nice >> system >> idle >> iowait >> irq >>
             softirq >> steal >> guest >> guest_nice) {
        int totalIdle = idle + iowait;
        int totalNonIdle = user + nice + system + irq + softirq + steal;
        int total = totalIdle + totalNonIdle;

        cpuUtilization0 =
            (total - totalIdle) / static_cast<double>(total) * 100.0;
      }
    }
  }
  return cpuUtilization;
}

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

  return totalProcesses;
}

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
  }

  return runningProcesses;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
    filestream.close();
  }

  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: "
              << kProcDirectory + std::to_string(pid) + kStatusFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  std::string ram;
  while (std::getline(filestream, line)) {
    std::istringstream iss(line);
    while (iss >> currentKey >> currentValue) {
      if (currentKey == "VmSize:") {
        ram = to_string( (int) std::floor(stof(currentValue) / 1024));
      }
      break;
    }
  }

  return ram;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: "
              << kProcDirectory + std::to_string(pid) + kStatusFilename
              << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  std::string uid;
  while (std::getline(filestream, line)) {
    std::istringstream iss(line);
    iss >> currentKey;
    if (currentKey == "Uid:") {
      iss >> currentValue;
      uid = currentValue;
      break;
    }
  }

  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string uid = Uid(pid);
  std::string name;
  std::ifstream filestream(kPasswordPath);
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: " << kPasswordPath << std::endl;
  }
  std::string temp;
  std::string user;
  std::string x;
  std::string line;
  std::string id;

  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream iss(line);
    iss >> user >> x >> id;
    if (id == uid) {
      name = user;
      break;
    }
  }

  return name;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long starttime = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file: "
              << kProcDirectory + std::to_string(pid) + kStatFilename
              << std::endl;
  }
  std::string line;
  std::string cuurentValue;
  vector<string> values;
  std::getline(filestream, line);
  std::istringstream iss(line);
  while (iss >> cuurentValue) {
    values.push_back(cuurentValue);
  }

  try {
    starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    starttime = 0;
  }
  return starttime;
}
