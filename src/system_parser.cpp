#include "system_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

vector<string> SystemParser::Lines(string filepath) {
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

// TODO: Refactor to use a map
float SystemParser::MemoryUtilization() {
  float mem_total{1};
  float mem_free{0};
  float buffers{0};
  string token;
  std::ifstream stream(Path::base + Path::meminfo);
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

long SystemParser::Active(vector<string> time) {
  float active{0};
  active += stol(time[CPUStates::kUser_]);
  return active +
         (stol(time[CPUStates::kNice_]) + stol(time[CPUStates::kSystem_]) +
          stol(time[CPUStates::kIRQ_]) + stol(time[CPUStates::kSoftIRQ_]) +
          stol(time[CPUStates::kSteal_]) + stol(time[CPUStates::kGuest_]) +
          stol(time[CPUStates::kGuestNice_]));
}

long SystemParser::Idle(vector<string> time) {
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

float SystemParser::CpuUtilization(vector<string> time1, vector<string> time2) {
  long active{Active(time2) - Active(time1)};
  long idle{Idle(time2) - Idle(time1)};
  long total{active + idle};
  float utilization = static_cast<float>(active) / total;
  return utilization;
}

vector<string> SystemParser::AggregateCpuUtilization() {
  string line;
  string token;
  vector<string> values;
  std::ifstream filestream(Path::base + Path::stat);
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

vector<vector<string>> SystemParser::IndividualCpuUtilizations() {
  string line;
  string token;
  vector<vector<string>> cpus;
  std::ifstream filestream(Path::base + Path::stat);
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

long int SystemParser::UpTime() {
  string token;
  std::ifstream stream(Path::base + Path::uptime);
  if (stream.is_open()) {
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}