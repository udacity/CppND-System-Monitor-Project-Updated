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
        //int pid = stoi(filename);
        try {
          long long int pid = stoi(filename);
        } catch (std::out_of_range& e) {
          std::cerr << "Number is out of range: " << e.what() << '\n';
        }
        long long int pid0 = stoi(filename);
        pids.push_back(pid0);
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
    std::cerr << "Failed to open file MemoryUtilization: "
              << kProcDirectory + kMeminfoFilename << std::endl;
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
  MemUtilization = (((memInfo[0]) - (MemAvailable)) / (memInfo[0]));

  return MemUtilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file UpTime: "
              << kProcDirectory + kUptimeFilename << std::endl;
  }
  long uptime;
  while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    std::string currentValue;
    iss >> currentValue;
    // uptime = std::stoi(currentValue);
    try {
      uptime = std::stoi(currentValue);
    } catch (std::out_of_range& e) {
      std::cerr << "Number is out of range: " << e.what() << '\n';
    }
    break;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0.0; }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    string line;
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::istream_iterator<string> streamIterator(linestream);
      std::istream_iterator<string> endIterator;
      // Skip the fields until the utime field
      std::advance(streamIterator, 12);
      // Read the utime field
      long utime = std::stol(*streamIterator++);
      // Read the stime field
      long stime = std::stol(*streamIterator++);
      activeJiffies = utime + stime;
    }
  }
  filestream.close();
  return activeJiffies;
}

float LinuxParser::cpuProcessUtilization(int pid) {
  long total_time;
  long seconds;
  float cpuUsage;
  long uptime = LinuxParser::UpTime();
  long starttimePerHz = LinuxParser::UpTime(pid);
  // Next we get the total elapsed time in seconds since the process started:
  total_time = LinuxParser::ActiveJiffies(pid);
  seconds = uptime - starttimePerHz;
  // Next we get the total elapsed time in seconds since the process started:
  cpuUsage = (float)100 * (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  return cpuUsage;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0.0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0.0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtilization;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string value;
      while (linestream >> value) {
        cpuUtilization.push_back(value);
      }
    }
  }

  filestream.close();
  return cpuUtilization;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file TotalProcesses: "
              << kProcDirectory + kStatFilename << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  long long int totalProcesses;
  while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    iss >> currentKey >> currentValue;
    if (currentKey == "processes") {
      // totalProcesses = std::stoi(currentValue);
      try {
        totalProcesses = std::stoi(currentValue);
      } catch (std::out_of_range& e) {
        std::cerr << "Number is out of range: " << e.what() << '\n';
      }
      break;
    }
    
  }

  return totalProcesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  if (!filestream.is_open()) {
    std::cerr << "Failed to open file RunningProcesses: "
              << kProcDirectory + kStatFilename << std::endl;
  }
  std::string currentKey;
  std::string currentValue;
  long long int runningProcesses;
  while (getline(filestream, line)) {
    std::istringstream iss(line);
    // read info
    iss >> currentKey >> currentValue;
    if (currentKey == "procs_running") {
      // runningProcesses = std::stoi(currentValue);
      try {
        runningProcesses = std::stoi(currentValue);
      } catch (std::out_of_range& e) {
        std::cerr << "Number is out of range: " << e.what() << '\n';
      }
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
    std::cerr << "Failed to open file Ram: "
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
        ram = to_string((int)std::floor(stof(currentValue) / 1024));
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
    std::cerr << "Failed to open file Uid: "
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
    std::cerr << "Failed to open file User: " << kPasswordPath << std::endl;
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
    std::cerr << "Failed to open file UpTime: "
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
  starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  return starttime;
}
