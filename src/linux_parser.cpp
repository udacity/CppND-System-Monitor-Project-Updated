#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  float total;
  float free = 0;
  std::ifstream filestream(kProcDirectory.substr(0, kProcDirectory.length() - 1) + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal") {
        linestream >> total;
      }
      if (key == "MemFree"){
        linestream >> free;
      }
      
    }
  }
  return (total - free)/total; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime = 0.0;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> upTime;
    }
  return upTime;
}

vector<long> GetJiffies(){
  vector<long> jiffies;
  string key, line;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        long jiffie;
        while (linestream>>jiffie){
            jiffies.push_back(jiffie);
        }
        break;
      }
    }
  }
  return jiffies;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long utime, stime, cutime, cstime;
  string output;
  string line;
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream (line);
    for(int i = 0; i < 14; i++){
      linestream >> output;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime+ cutime + cstime; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<long> jiffies = GetJiffies();
  long user = jiffies[0];
  long nice = jiffies[1];
  long system = jiffies[2];
  long irq = jiffies[5];
  long softirq = jiffies[6];
  long steal = jiffies[7];
  long guest = jiffies[8];
  long guest_nice = jiffies[9];

  long usertime = user - guest;
  long nicetime = nice - guest_nice;
  long systemalltime = system + irq + softirq;
  long virtalltime = guest + guest_nice;

  return usertime + nicetime + systemalltime + virtalltime; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<long> jiffies = GetJiffies();
  long idle = jiffies[3];
  long iowait = jiffies[4];
  return idle + iowait; 
}

// Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  return (float)ActiveJiffies()/(float)Jiffies();
}

string checkFileForString(std::string filepath, std::string searchKey){
  string line;
  string key;
  string output = "";
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(':') != std::string::npos){
        std::replace(line.begin(), line.end(), ':', ' ');
      }
      std::istringstream linestream(line);
      linestream >> key;
      if (key == searchKey) {
        linestream >> output;
        break;
      }
    }
  }
  return output; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stoi(checkFileForString(kProcDirectory + kStatFilename, "processes"));
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(checkFileForString(kProcDirectory + kStatFilename, "procs_running")); 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  
  return line; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  long int ram = std::stol(checkFileForString(kProcDirectory + std::to_string(pid) + kStatusFilename, "VmSize"));
  
  return std::to_string(ram*0.001);  
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  return checkFileForString(kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid"); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line, username, x, current_uid;
  std::ifstream filestream (kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> current_uid;
      if (uid == current_uid){
        return username;
      }
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string uptime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++){
          linestream >> uptime;
    }
  } 
  return std::stol(uptime)/sysconf(_SC_CLK_TCK); 
}