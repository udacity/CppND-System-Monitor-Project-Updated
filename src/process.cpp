#include <math.h>
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

Process::Process(int pid_) { pid = pid_; }

int Process::Pid() { return pid; }

float Process::CpuUtilization() {
  /*
  We apply the CPU usage algorithm suggested by Vilhelm Gray on
  https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  */
  string line;
  string key;
  string value;

  int uptime = LinuxParser::UpTime();

  long int utime, stime, cutime, cstime, starttime = 0;

  vector<string> data = {};
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string stat;

      // unpack to a vector
      while (linestream >> stat) {
        data.emplace_back((stat));
      }
    }
  }
  // as the order is fixed, we know the position of the desired data
  utime = stol(data[13]);
  stime = stol(data[14]);
  cutime = stol(data[15]);
  cstime = stol(data[16]);
  starttime = stol(data[21]);
  int hertz = sysconf(_SC_CLK_TCK);
  int total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (static_cast<float>(starttime) / hertz);
  float cpu_usage = 0.0;

  cpu_usage = ((static_cast<float>(total_time) / hertz) / seconds);
  last_cpu = cpu_usage;
  return cpu_usage;
}

long Process::ActiveJiffies(int pid) {
  string line;
  string key;
  string value;

  vector<string> data = {};
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string stat;

      // unpack to a vector
      while (linestream >> stat) {
        data.emplace_back((stat));
      }
    }
  }
  // as the order is fixed, we know the position of the desired data
  return stol(data[13]);
}

string Process::Command() {
  string line;
  string read_value = {};

  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> read_value;
    }
  }
  // check for size and prune for readability
  if (read_value.size() > 40) {
    read_value = read_value.substr(0, 37);
    read_value += "...";
  }
  return read_value;
}

string Process::Ram() {
  string line;
  string key = {};
  string value = {};
  string used_mem = {};
  int mem_mb = 0;

  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        /* using vmrss to get the psysical ram as per reviewers suggestion */
        if (key == LinuxParser::filterProcMem) {
          /* implict int conversion by using the mem_mb variable */
          mem_mb = std::round(stoi(value) * 0.001);
          used_mem = to_string(mem_mb);
        }
      }
    }
  }

  return used_mem;
}

string Process::Uid() {
  string line;
  string key = {};
  string value = {};
  string uid = {};
  string user = {};
  string uid_ref = {};

  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> uid) {
        if (key == LinuxParser::filterUID) {
          return uid;
          break;
        }
      }
    }
  }
  return string();
}

string Process::User() {
  string line;
  string key = {};
  string value = {};
  string uid = {};
  string user = {};
  string pwd_aux = {};
  string uid_ref = {};
  uid = Process::Uid();

  std::ifstream filestream_pwd(LinuxParser::kPasswordPath);
  if (filestream_pwd.is_open()) {
    while (std::getline(filestream_pwd, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> pwd_aux >> key >> uid_ref) {
        if (key == uid) {
          return user;
        }
      }
    }
  }
  return string();
}

long int Process::UpTime() {
  string line;
  string key;
  string value;
  vector<string> data = {};
  long int age = 0;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string stat;

      // unpack to a vector
      while (linestream >> stat) {
        data.emplace_back((stat));
      }
    }
  }
  age = LinuxParser::UpTime() - (stol(data[21]) / sysconf(_SC_CLK_TCK));

  return age;
  // return 0;
}

bool Process::operator<(Process const& a) const {
  return last_cpu < a.last_cpu;
}