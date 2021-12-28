#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

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

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    // Defining as float here to get the correct precision on the
    //  percentage of use calculation
    float avalilabe_mem, free_mem = 0.0;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          avalilabe_mem = stoi(value);
        }
        if (key == "MemFree") {
          free_mem = stoi(value);
        }
      }
    }

    return (avalilabe_mem - free_mem) / avalilabe_mem;
  }
  return 0.0;
}

long LinuxParser::UpTime() {
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  } else {
    uptime = '0';
  }
  return stol(uptime);
}

vector<long int> LinuxParser::JiffiesInfos() {
  string line;
  string key;
  string value;
  string stat;
  vector<long int> cpu_usages;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> stat) {
            cpu_usages.push_back(stol(stat));
          }
        }
      }
    }
  }
  return cpu_usages;
}

long LinuxParser::Jiffies() { return LinuxParser::JiffiesInfos()[0]; }

long LinuxParser::ActiveJiffies(int pid) {
  Process proc = Process(pid);

  return proc.ActiveJiffies(pid);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::JiffiesInfos()[0] - LinuxParser::JiffiesInfos()[3];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return LinuxParser::JiffiesInfos()[3]; }

vector<long int> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string stat;
      vector<long int> cpu_usages;
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> stat) {
            cpu_usages.push_back(stol(stat));
          }
          return cpu_usages;
        }
      }
    }

    return {};
  }
  return {};
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    int processes = 0;
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = stoi(value);
        }
      }
    }

    return processes;
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    int processes = 0;
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "procs_running") {
          processes = stoi(value);
        }
      }
    }

    return processes;
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  Process proc = Process(pid);

  return proc.Uid();
}

string LinuxParser::Ram(int pid) {
  Process proc = Process(pid);

  return proc.Ram();
}

string LinuxParser::Uid(int pid) {
  Process proc = Process(pid);

  return proc.Uid();
}

string LinuxParser::User(int pid) {
  Process proc = Process(pid);

  return proc.User();
}

long LinuxParser::UpTime(int pid) {
  Process proc = Process(pid);

  return proc.UpTime();
}
