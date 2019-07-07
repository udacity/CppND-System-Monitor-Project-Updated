#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

vector<string> LinuxParser::Lines(string filepath) {
  string line;
  vector<string> lines;
  std::ifstream stream(filepath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      lines.push_back(line);
    }
  }
  return lines;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir("/proc");
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
  float mem_total{1};
  float mem_free{0};
  float buffers{0};
  string token;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "MemTotal:") {
        if (stream >> token) mem_total = stof(token);
      } else if (token == "MemFree:") {
        if (stream >> token) mem_free = stof(token);
      } else if (token == "Buffers:") {
        if (stream >> token) buffers = stof(token);
      }
    }
  }
  return 1 - mem_free / (mem_total - buffers);
}

long LinuxParser::UpTime() {
  string token;
  for (string& line : LinuxParser::Lines(LinuxParser::kProcDirectory +
                                         LinuxParser::kUptimeFilename)) {
    std::istringstream stream(line);
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }
  long jiffies{0};
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

long LinuxParser::ActiveJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
          stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
          stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
          stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
}

long LinuxParser::IdleJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string token;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token) {
        if (token == "cpu") {
          while (linestream >> token) values.push_back(token);
          return values;
        }
      }
    }
  }
  return values;
}

vector<vector<string>> LinuxParser::IndividualCpuUtilizations() {
  string line;
  string token;
  vector<vector<string>> cpus;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    int i = 0;
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token) {
        string cpu{"cpu" + to_string(i)};
        if (token == cpu) {
          cpus.push_back({});
          while (linestream >> token) cpus[i].push_back(token);
          ++i;
        }
      }
    }
  }
  return cpus;
}