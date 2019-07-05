#include "process_parser.h"
#include <dirent.h>
#include <cmath>
#include <locale>
#include <string>
#include <vector>
#include "util.h"

using std::string;
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
  std::ifstream stream(Path::base_path + pid + Path::status_path);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "VmSize:") {
        stream >> token;
        return std::to_string(stoi(token)/1024);
      }
    }
  }
  return string("NA");
}

string ProcessParser::Cmd(string pid) { return "/" + pid; }

string ProcessParser::Cpu(string pid) { return "0" + pid; }

string ProcessParser::UpTime(string pid) { return "0" + pid; }

string ProcessParser::User(string pid) { return "0" + pid; }

long int ProcessParser::getSysUpTime() { return 0; }
vector<string> ProcessParser::getSysCpuPercent(string coreNumber) {
  return vector<string>{coreNumber};
}
float ProcessParser::getSysRamPercent() { return 0; }
string ProcessParser::getSysKernelVersion() { return string(""); }
int ProcessParser::getNumberOfCores() { return 0; }
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