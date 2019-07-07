#include "process.h"
#include <system_parser.h>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>

using std::string;
using std::to_string;
using std::vector;

Process::Process(string pid) : pid_(pid) {}

string Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpu_; }

void Process::CpuUtilization(float cpu) { cpu_ = cpu; }

string Process::Command() const {
  string line;
  std::ifstream stream(Path::base + pid_ + Path::cmdline);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    return line;
  }
  return "";
}

string Process::Ram() const {
  string token;
  std::ifstream stream(Path::base + pid_ + Path::status);
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
  std::ifstream stream(Path::base + pid_ + Path::status);
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
  std::ifstream stream(Path::users);
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

string Process::UpTime() const {
  string token;
  std::ifstream stream(Path::base + pid_ + Path::stat);
  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i)
      if (i == 13) {
        int time{stoi(token)};
        time /= sysconf(_SC_CLK_TCK);
        return to_string(time);
      }
  }
  return token;
}

long Process::Jiffies() const {
  string line, token;
  vector<string> values;
  std::ifstream filestream(Path::base + pid_ + Path::stat);
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

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}