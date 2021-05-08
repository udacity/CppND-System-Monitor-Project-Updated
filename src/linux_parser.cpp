#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

namespace fs = std::filesystem;

// creates userid->username mapping
// assuming users don't change during program run
unordered_map<int, string> LinuxParser::Usernames() {
  unordered_map<int, string> id2name;
  string line;
  string name;
  string epwd;
  int id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line);
      if (ls >> name >> epwd >> id) {
        id2name[id]=name;
      }
    }
  }
  return id2name;
}

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
      std::istringstream ls(line);
      while (ls >> key >> value) {
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
    std::istringstream ls(line);
    ls >> os >> version >> kernel;
  }
  return kernel;
}

unordered_set<int> LinuxParser::Pids() {
  unordered_set<int> pids;
  string dirName;
  for(auto& p : fs::directory_iterator(kProcDirectory)) {
    if (fs::is_directory(p)) {
      dirName = p.path().filename();
      if (std::all_of(dirName.begin(), dirName.end(), isdigit)) {
        pids.insert(stoi(dirName));
      }
    }
  }
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
  long uptimeSec = 0L;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    std::istringstream ls(line);
    ls >> uptimeSec;
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
    long jiffs[8] = {0L};
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
          .totalJiffies=0L
        };
        cj.totalJiffies=cj.activeJiffies+jiffs[LinuxParser::kIdle_]+jiffs[LinuxParser::kIOwait_];
        cpuJiffies.push_back(cj);
      }
    }
  }
  return cpuJiffies;
}

// Read and return the number of active jiffies for a PID
// measured in clock ticks by adding utime, stime, cutime, cstime
// as well as start time in clock ticks
LinuxParser::PidJiffies LinuxParser::ProcessJiffies(const int pid) {
  LinuxParser::PidJiffies pj = { .activeJiffies=0L, .startTime=0L };
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    std::istringstream ls(line);
    string val;
    for (int i=0;i<13 && (ls >> val); ++i);
    long jiff = 0L;
    for (int i=0;i<4 && (ls >> jiff); ++i) {
      pj.activeJiffies+=jiff;
    }
    for (int i=0;i<4 && (ls >> jiff); ++i);
    ls >> pj.startTime;
  }
  return pj;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcs = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    string line;
    string key;
    while (std::getline(filestream, line)) {
      std::istringstream ls(line);
      if ((ls >> key) && key == "processes") {
        ls >> totalProcs;
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
      std::istringstream ls(line);
      if ((ls >> key) && key == "procs_running") {
        ls >> runningProcs;
        return runningProcs;
      }
    }
  }
  return runningProcs;
}

// Read and return the command associated with a process
string LinuxParser::Command(const int pid) {
  string cmd;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd);
  }
  return cmd;
}

// Read and return the memory used by a process
// assuming value is expressed in KB
int LinuxParser::Ram(const int pid) {
  int vmSize=0;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    string line;
    string val;
    while (std::getline(filestream, line)) {
      std::istringstream ls(line);
      if ((ls >> val) && val == "VmSize:") {
        ls >> vmSize;
        return vmSize;
      }
    }
  }
  return vmSize;
}

// Read and return the user ID associated with a process
int LinuxParser::Uid(const int pid) {
  int uid=0;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    string line;
    string val;
    while (std::getline(filestream, line)) {
      std::istringstream ls(line);
      if ((ls >> val) && val == "Uid:") {
        ls >> uid;
        return uid;
      }
    }
  }
  return uid;
}
