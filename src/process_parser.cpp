#include <dirent.h>
#include <cmath>
#include <fstream>
#include <locale>
#include <string>
#include <vector>

#include "process_parser.h"
#include "util.h"

using std::string;
using std::to_string;
using std::vector;

// BONUS: Upgrade this to use C++17 std::filesystem
vector<string> ProcessParser::getPidList() {
  vector<string> list;
  DIR* directory = opendir("/proc");
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        list.push_back(filename);
      }
    }
  }
  closedir(directory);
  return list;
}

string ProcessParser::VmSize(string pid) {
  string token;
  std::ifstream stream(Path::base + pid + Path::status);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "VmSize:") {
        if (stream >> token) return std::to_string(stoi(token) / 1024);
      }
    }
  }
  return string("NA");
}

string ProcessParser::Cmdline(string pid) {
  string line;
  std::ifstream stream(Path::base + pid + Path::cmdline);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    return line;
  }
  return "NA";
}

// TODO: Calculate CPU
string ProcessParser::Cpu(string pid) {
  string token;
  std::ifstream stream(Path::base + pid + Path::stat);
  if (stream.is_open()) {
  }
  return "NA";
}

string ProcessParser::UpTime(string pid) {
  string token;
  std::ifstream stream(Path::base + pid + Path::stat);
  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i)
      if (i == 13) {
        int time{stoi(token)};
        time /= sysconf(_SC_CLK_TCK);
        return to_string(time);
      }
  }
  return "NA";
}

string ProcessParser::Uid(string pid) {
  string token;
  std::ifstream stream(Path::base + pid + Path::status);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "Uid:") {
        if (stream >> token) return token;
      }
    }
  }
  return string("");
}

string ProcessParser::User(string pid) {
  std::ifstream stream(Path::users);
  if (stream.is_open()) {
    string line;
    string token = "x:" + ProcessParser::Uid(pid);
    while (std::getline(stream, line)) {
      auto marker = line.find(token);
      if (marker != string::npos) {
        return line.substr(0, marker - 1);
      }
    }
  }
  return "NA";
}

long int ProcessParser::getSysUpTime() { return 0; }
vector<string> ProcessParser::getSysCpuPercent(string coreNumber) {
  return vector<string>{coreNumber};
}
float ProcessParser::getSysRamPercent() { return 0; }
string ProcessParser::getSysKernelVersion() { return string(""); }
int ProcessParser::getTotalThreads() { return 0; }
int ProcessParser::getTotalNumberOfProcesses() { return 0; }
int ProcessParser::getNumberOfRunningProcesses() { return 0; }
string ProcessParser::getOSName() { return string(""); }
string ProcessParser::PrintCpuStats(vector<string> values1,
                                    vector<string> values2) {
  string value;
  if (values1.size() > 0) value += values1[0];
  if (values2.size() > 0) value += values2[0];
  return value;
}
bool ProcessParser::isPidExisting(string pid) { return pid == pid; }