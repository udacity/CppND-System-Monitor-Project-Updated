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

// TODO
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO
long LinuxParser::UpTime() { return 0; }

// TODO
long LinuxParser::Jiffies() { return 0; }

// TODO
long LinuxParser::ActiveJiffies(int pid) {
  pid = pid;
  return 0;
}

// TODO
long LinuxParser::ActiveJiffies() { return 0; }

// TODO
long LinuxParser::IdleJiffies() { return 0; }

// TODO
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO
int LinuxParser::TotalProcesses() { return 0; }

// TODO
int LinuxParser::RunningProcesses() { return 0; }

// TODO
string LinuxParser::Command(int pid) {
  pid = pid;  // Prevent compiler warnings about `int pid` being unused
  return string();
}

// TODO
string LinuxParser::Ram(int pid) {
  pid = pid;  // Prevent compiler warnings about `int pid` being unused
  return string();
}

// TODO
string LinuxParser::Uid(int pid) {
  pid = pid;  // Prevent compiler warnings about `int pid` being unused
  return string();
}

// TODO
string LinuxParser::User(int pid) {
  pid = pid;  // Prevent compiler warnings about `int pid` being unused
  return string();
}

// TODO
long LinuxParser::UpTime(int pid) {
  pid = pid;  // Prevent compiler warnings about `int pid` being unused
  return 0;
}