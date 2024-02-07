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
  MemUtilization = (((memInfo[0]) - (MemAvailable)) / (memInfo[0]));

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
long LinuxParser::Jiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  std::string key;
  long total_jiffies = 0;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        total_jiffies = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
        break;
      }
    }
  }

  return total_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
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

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    long activeJiffies = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string cpuLabel;
      linestream >> cpuLabel; // Skip the CPU label
      long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
      linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
      activeJiffies = user + nice + system + irq + softirq + steal + guest + guest_nice;
    }
  }

  filestream.close();
  return activeJiffies;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line;
  std::string key;
  long idle_jiffies = 0;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        idle_jiffies = idle;
        break;
      }
    }
  }

  return idle_jiffies;
}

// float LinuxParser::ProcessCpuUtilization(int pid) {
//   long total_jiffies_start = LinuxParser::Jiffies();
//   long process_jiffies_start = LinuxParser::ActiveJiffies(pid);

//   // Sleep for a short duration to get the time difference
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));

//   long total_jiffies_end = LinuxParser::Jiffies();
//   long process_jiffies_end = LinuxParser::ActiveJiffies(pid);

//   // Calculate the difference in jiffies
//   long total_jiffies_diff = total_jiffies_end - total_jiffies_start;
//   long process_jiffies_diff = process_jiffies_end - process_jiffies_start;

//   // Calculate the CPU utilization
//   float cpu_utilization = static_cast<float>(process_jiffies_diff) / total_jiffies_diff;

//   return cpu_utilization;
// }

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



