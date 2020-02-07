#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>

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
  string os, versionText, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> versionText >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  /*
  alend@alend-VirtualBox:~/CppND-System-Monitor-Project-Updated$ cat /proc/meminfo 
  MemTotal:        1008868 kB
  MemFree:          308404 kB
  MemAvailable:     365316 kB
  Buffers:           16420 kB
  ..
  */
  long memTotal, memFree, value;
  string line, titleText, unitText;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> titleText >> value >> unitText;
      if (titleText == "MemTotal:") {
        memTotal = value;
      }
      if (titleText == "MemFree:") {
        memFree = value;
      }
    }
  }
  return (memTotal-memFree)*1.0/memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string token;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    if (linestream >> token) {
      return std::stol(token);
    }
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long upTime = UpTime(); //in seconds
  long tick = sysconf(_SC_CLK_TCK); //1 jiffy per second
  return upTime * tick;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {

  vector<string> times; 
  string line;
  string text;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> text) {
      times.push_back(text);
    }
  }

  return stol(times[13]) + stol(times[14])+ stol(times[15])+ stol(times[16]); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<string> utilizationTimes = CpuUtilization();
    long totalActiveJiffies = stol(utilizationTimes[CPUStates::kUser_]) + stol(utilizationTimes[CPUStates::kNice_]) +
      stol(utilizationTimes[CPUStates::kSystem_]) + stol(utilizationTimes[CPUStates::kIRQ_]) +
      stol(utilizationTimes[CPUStates::kSoftIRQ_]) + stol(utilizationTimes[CPUStates::kSteal_]) +
      stol(utilizationTimes[CPUStates::kGuest_]) + stol(utilizationTimes[CPUStates::kGuestNice_]);
  return totalActiveJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
 vector<string> utilizationTimes = CpuUtilization();
 return stol(utilizationTimes[CPUStates::kIdle_]);
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0; 

  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0; 

 }


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {

  string line;
  vector<string> tokens;
  string token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token) {
        if (token == "cpu") {
          while (linestream >> token) {
            tokens.push_back(token);
          }
          return tokens;
        }
      }
    }
  }
  return tokens;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }  
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        long ram = std::stol(value);
        return std::to_string(ram / 1024); // in MB
      }
    }   
  }
  return "n/a";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }   
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line, user, ignore, key;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> ignore >> key) {
        if (key == uid) {
          return user;
        }
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) {
  long int upTime;
  std::string ignore;
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> ignore 
      >> ignore >> ignore >> ignore >> ignore  >> ignore
      >> ignore >> ignore >> ignore >> ignore  >> ignore
      >> ignore >> ignore >> ignore >> ignore  >> ignore
      >> ignore >> ignore >> ignore >> ignore  >> ignore
      >> upTime;
    }
  return upTime/sysconf(_SC_CLK_TCK);
}
