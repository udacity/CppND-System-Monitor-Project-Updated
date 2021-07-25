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

float LinuxParser::MemoryUtilization() { 
  string sTotal, sFree;
  unsigned int nTotal, nFree, nUsed;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> sTotal >> nTotal;
    std::getline(stream,line);
    linestream.str(line);
    linestream >> sFree >> nFree;
  }
  nUsed = nTotal - nFree;
  return float(nUsed)/float(nTotal); 
}

long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), '.', ' ');
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }
  return 0; 
}

long LinuxParser::Jiffies() { 
  auto cpuData = CpuUtilization();

  long jiffies = 0;
  for(auto value: cpuData) jiffies += value;

  return jiffies; 
}

long LinuxParser::ActiveJiffies(int pid) { 
  long active_jiffies = 0;
  auto stat_values = Stat(pid);
  for(int i = 13; i < 17; i++) {
    active_jiffies += std::stol(stat_values.at(i).c_str());
  }
  return active_jiffies; 
}

long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies(); 
}

long LinuxParser::IdleJiffies() { 
  auto cpuData = CpuUtilization();
  long idle_jiffies = 0;
  idle_jiffies = cpuData[kIdle_] + cpuData[kIOwait_];
  return idle_jiffies;
}

vector<unsigned int> LinuxParser::CpuUtilization() { 
  string token;
  vector<unsigned int> cpuData(size_t(10),0);
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if(token == "cpu") {
        for(std::size_t i = 0; i < cpuData.size(); i++) 
          linestream >> cpuData[i];
        cpuData[kUser_] -= cpuData[kGuest_];
        cpuData[kNice_] -= cpuData[kGuestNice_];
        return cpuData;
      }
    }
  }
  return cpuData; 
}

int LinuxParser::TotalProcesses() { 
  string token;
  unsigned int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == "processes") {
        linestream >> value;
        return value;
      }
    }
  }
  return 0; 
}

int LinuxParser::RunningProcesses() {   
  string token;
  unsigned int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == "procs_running") {
        linestream >> value;
        return value;
      }
    }
  }
  return 0; 
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream;
  std::istringstream linestream;
  stream.open(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string(); 
}

string LinuxParser::Ram(int pid) { 
  string line, token; 
  long ramkB;
  std::ifstream stream;
  std::istringstream linestream;
  stream.open(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      linestream.str(line);
      linestream >> token;
      if(token == "VmSize:") {
        linestream >> ramkB;
        return to_string(ramkB / 1000);
      }
    }
  }
  return string(); 
}

int LinuxParser::Uid(int pid) { 
  string line, token;
  int uid;
  std::ifstream stream;
  std::istringstream linestream;
  stream.open(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      linestream.str(line);
      linestream >> token;
      if(token == "Uid:") {
        linestream >> uid;
        return uid;
      }
    }
  }
  return 0; 
}

string LinuxParser::User(int pid) { 
  string line, token, user;
  int uid, ntoken;
  uid = Uid(pid);
  std::ifstream stream;
  std::istringstream linestream;
  stream.open(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      linestream.str(line);
      linestream >> user >> token >> ntoken;
      if (ntoken == uid) {
        return user;
      }
    }
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  long ticks;
  if(std::all_of(Stat(pid).at(21).begin(), Stat(pid).at(21).end(), isdigit)) {
    ticks = std::stol(Stat(pid).at(21).c_str());
    return long( ticks / sysconf(_SC_CLK_TCK) ); 
  }
  return 0;
}

vector<string> LinuxParser::Stat(int pid) {
  string line, token;
  vector<string> stat_values = {};
  std::ifstream stream;
  std::istringstream linestream;
  stream.open(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    linestream.str(line);
    while(!linestream.eof()) {
      linestream >> token;
      stat_values.emplace_back(token);
    }
  }
  return stat_values;
}
