#include "process_parser.h"
#include "system_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::string SystemParser::TotalThreads() {
  vector<string> pids = ProcessParser::Pids();
  int total = 0;
  for (string& pid : pids) {
    total += stoi(ProcessParser::Threads(pid));
  }
  return std::to_string(total);
}

std::string SystemParser::TotalProcesses() {
  string line, key, value;
  std::ifstream stream(Path::base + Path::stat);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return "NA";
}

std::string SystemParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(Path::base + Path::stat);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return "NA";
}

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

std::string SystemParser::CpuUtilization(vector<string> time1,
                                         vector<string> time2) {
  long active{Active(time2) - Active(time1)};
  long idle{Idle(time2) - Idle(time1)};
  long total{active + idle};
  float utilization = static_cast<float>(active) / total * 100;
  return to_string(utilization);
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