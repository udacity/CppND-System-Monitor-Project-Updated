#include <dirent.h>
//#include <filesystem>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read and return OS name
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

// Read and return kernel
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// This function returns Pid's of processes using std::filesystem.
// Has been commented out because not compiling using c++17
/*
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto& p: std::filesystem::directory_iterator("kProcDirectory)){
    if (p.is_directory()) {
      string path = p.path();
      size_t found = path.find_last_of("/\\");
      string foldername = path.substr(found+1);
      if (std::all_of(foldername.begin(), foldername.end(), isdigit)) {
        int pid = stoi(foldername);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}
*/

// Read and return Pid's of processes
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
  float result = 0;
  long memory[2];  //{total, free}
  string line, temp;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 2; i++) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), '_', ' ');
      std::istringstream linestream(line);
      linestream >> temp >> memory[i];
    }
    // Calculating memory utilization as (memtotal-memfree)/memtotal
    result = static_cast<float>(memory[0] - memory[1]) / memory[0];
  }
  return result;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long value = 0, temp = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> temp) value += temp;
  }
  return value;
}

// Read and return CPU utilization
LinuxParser::systemCPUStat LinuxParser::CpuUtilization() {
  string line;
  string key;
  LinuxParser::systemCPUStat result;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    linestream >> result.user_ >> result.nice_ >> result.system_ >>
        result.idle_ >> result.ioWait_ >> result.irq_ >> result.softIRQ_ >>
        result.steal_;
  }
  return result;
}

// Read and return CPU utilization for a process
LinuxParser::procCPUStat LinuxParser::CpuUtilization(int pid) {
  string line;
  string temp;
  LinuxParser::procCPUStat result;
  std::stringstream ss;
  ss << kProcDirectory << "/" << std::to_string(pid) << "/" << kStatFilename;
  std::ifstream filestream(ss.str());
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 14; i++) {
      linestream >> temp;
    }
    linestream >> result.utime_  // 14
        >> result.stime_         // 15
        >> result.cutime_        // 16
        >> result.cstime_;       // 17
  }
  return result;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> value;
        return value;
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> value;
        return value;
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::stringstream ss;
  ss << kProcDirectory << "/" << std::to_string(pid) << "/" << kCmdlineFilename;
  std::ifstream filestream(ss.str());
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  int mem = 0;
  std::stringstream ss;
  ss << kProcDirectory << "/" << std::to_string(pid) << "/" << kStatusFilename;
  std::ifstream filestream(ss.str());
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> mem;
      if (key == "VmSize:") {
        mem /= 1024;
        return std::to_string(mem);
      }
    }
  }
  return "";
}

// Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
  string line;
  string key;
  int value = 0;
  std::stringstream ss;
  ss << kProcDirectory << "/" << std::to_string(pid) << "/" << kStatusFilename;
  std::ifstream filestream(ss.str());
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int uid) {
  string line;
  string user;
  string temp;
  int value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> temp >> value;
      if (value == uid) {
        return user;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  long result = 0;
  std::stringstream ss;
  ss << kProcDirectory << "/" << std::to_string(pid) << "/" << kStatFilename;
  std::ifstream filestream(ss.str());
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 22; i++) {
      linestream >> key;
    }
    linestream >> result;
  }
  return result;
}