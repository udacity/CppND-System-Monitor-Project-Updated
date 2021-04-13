#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
float LinuxParser::MemoryUtilization() {
  int memTotal = -1;
  int memFree = -1;
  std::ifstream fs(kProcDirectory+kMeminfoFilename);
  if (fs.is_open()) {
    string line;
    string key;
    while (std::getline(fs, line) && (memFree==-1 || memTotal==-1)) {
      std::istringstream ls(line);
      ls >> key;
      if (key == "MemTotal:") {
         ls >> memTotal;
      }
      else if (key == "MemFree:") {
         ls >> memFree;
      }
    }
  }
  return (memTotal<=0 || memFree==-1 ? 0.0f : (memTotal-memFree)/(float)memTotal);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptimeSec = 0;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptimeSec;
  }
  return uptimeSec;
}

// Read and return active, total jiffies for all cpus
vector<LinuxParser::CpuJiffies> LinuxParser::AllJiffies() {
  vector<LinuxParser::CpuJiffies> cpuJiffies {};
  std::ifstream fs(kProcDirectory+kStatFilename);
  if (fs.is_open()) {
    string line;
    string key;
    long jiffs[8] = {0};
    while (std::getline(fs, line)) {
      std::istringstream ls(line);
      if ((ls >> key) && key.rfind("cpu", 0) == 0) {
        for (int i=0; i<8; ++i) {
          ls >> jiffs[i];
        }
        LinuxParser::CpuJiffies cj = {
          .cpuID=key,
          .activeJiffies=jiffs[LinuxParser::kUser_]+jiffs[LinuxParser::kNice_]+
                    jiffs[LinuxParser::kSystem_]+jiffs[LinuxParser::kIRQ_]+
                    jiffs[LinuxParser::kSoftIRQ_]+jiffs[LinuxParser::kSteal_],
          .totalJiffies=0
        };
        cj.totalJiffies=cj.activeJiffies+jiffs[LinuxParser::kIdle_]+jiffs[LinuxParser::kIOwait_];
        cpuJiffies.push_back(cj);
      }
    }
  }
  return cpuJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcs = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    string line;
    string key;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && key == "processes") {
        linestream >> totalProcs;
        return totalProcs;
      }
    }
  }
  return totalProcs;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcs = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    string line;
    string key;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && key == "procs_running") {
        linestream >> runningProcs;
        return runningProcs;
      }
    }
  }
  return runningProcs;
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
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
