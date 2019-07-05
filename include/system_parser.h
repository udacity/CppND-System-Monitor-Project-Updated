#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "constants.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

class SystemParser {
 public:
  static long int UpTime();
  static int CpuCores();
  static string Cpu(vector<string> time1, vector<string> time2);
  static float MemoryUtilization();
  static std::string Kernel();
  static std::string OperatingSystem();

 private:
  static float Active(vector<string> cpu);
  static float Idle(vector<string> cpu);
  enum CPUStates {
    kUser_ = 1,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
  static CPUStates cpu_states_;
};

std::string SystemParser::OperatingSystem() {
  string line, key, value;
  std::ifstream stream("/etc/os-release");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return "NA";
}

std::string SystemParser::Kernel() {
  string token{"NA"};
  std::ifstream stream(Path::base + Path::version);
  if (stream.is_open()) {
    for (int i = 0; i <= 2; ++i) {
      stream >> token;
    }
  }
  return token;
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
  return (1 - mem_free / (mem_total - buffers)) * 100;
}

float SystemParser::Active(vector<string> time) {
  return (stof(time[CPUStates::kUser_]) + stof(time[CPUStates::kNice_]) +
          stof(time[CPUStates::kSystem_]) + stof(time[CPUStates::kIRQ_]) +
          stof(time[CPUStates::kSoftIRQ_]) + stof(time[CPUStates::kSteal_]) +
          stof(time[CPUStates::kGuest_]) + stof(time[CPUStates::kGuestNice_]));
}

float SystemParser::Idle(vector<string> time) {
  return (stof(time[CPUStates::kIdle_]) + stof(time[CPUStates::kIOwait_]));
}

std::string SystemParser::Cpu(vector<string> time1, vector<string> time2) {
  float active = Active(time2) - Active(time1);
  float idle = Idle(time2) - Idle(time1);
  float total = active + idle;
  float utilization = active / total * 100;
  return to_string(utilization);
}

int SystemParser::CpuCores() {
  string token;
  std::ifstream stream(Path::base + Path::cpuinfo);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "cores") {
        if (stream >> token >> token) return stoi(token);
      }
    }
  }
  return 0;
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

#endif