#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpu_; }

void Process::CpuUtilization(long active_ticks, long system_ticks) {
  long duration_active{active_ticks - cached_active_ticks_};
  long duration{system_ticks - cached_system_ticks_};
  cpu_ = static_cast<float>(duration_active) / duration;
  cached_active_ticks_ = active_ticks;
  cached_system_ticks_ = system_ticks;
}

string Process::Command() const {
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) +
                       LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    return line;
  }
  return "";
}

string Process::Ram() const {
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) +
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "VmSize:") {
        if (stream >> token) return std::to_string(stoi(token) / 1024);
      }
    }
  }
  return string("0");
}

string Process::Uid() const {
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) +
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "Uid:") {
        if (stream >> token) return token;
      }
    }
  }
  return string("");
}

string Process::User() const {
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()) {
    string line;
    string token = "x:" + Uid();
    while (std::getline(stream, line)) {
      auto marker = line.find(token);
      if (marker != string::npos) {
        return line.substr(0, marker - 1);
      }
    }
  }
  return "0";
}

long int Process::UpTime() const {
  long int time{0};
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i)
      if (i == 13) {
        long int time{stol(token)};
        time /= sysconf(_SC_CLK_TCK);
        return time;
      }
  }
  return time;
}

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}