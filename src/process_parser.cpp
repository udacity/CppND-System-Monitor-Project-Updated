#include "process_parser.h"
#include <dirent.h>
#include <cmath>
#include <locale>
#include <string>
#include <vector>
#include "util.h"

std::string ProcessParser::getCmd(std::string pid) { return std::string(); }

// BONUS: Upgrade this to use C++17 std::filesystem
std::vector<std::string> ProcessParser::getPidList() {
  std::vector<std::string> list;
  DIR* directory = opendir("/proc");
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      std::string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        list.push_back(filename);
      }
    }
  }
  closedir(directory);
  return list;
}

// TODO: Refactor
std::string ProcessParser::VmSize(std::string pid) {
  std::string line;
  std::string name{"VmData"};

  std::ifstream stream;
  Util::GetStream(Path::base_path + pid + Path::status_path, stream);
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream buf(line);
      std::istream_iterator<std::string> beg(buf), end;
      std::vector<std::string> values(beg, end);
      return std::to_string(stof(values[1]) / pow(1024, 2));
    }
  }

  return std::string("");
}

std::string ProcessParser::getCpuPercent(std::string pid) {
  return std::string();
}
long int ProcessParser::getSysUpTime() { return 0; }
std::string ProcessParser::getProcUpTime(std::string pid) { return 0; }
std::string ProcessParser::getProcUser(std::string pid) { return 0; }
std::vector<std::string> ProcessParser::getSysCpuPercent(
    std::string coreNumber) {
  return std::vector<std::string>{};
}
float ProcessParser::getSysRamPercent() { return 0; }
std::string ProcessParser::getSysKernelVersion() { return std::string(""); }
int ProcessParser::getNumberOfCores() { return 0; }
int ProcessParser::getTotalThreads() { return 0; }
int ProcessParser::getTotalNumberOfProcesses() { return 0; }
int ProcessParser::getNumberOfRunningProcesses() { return 0; }
std::string ProcessParser::getOSName() { return std::string(""); }
std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1,
                                         std::vector<std::string> values2) {
  return std::string("");
}
bool ProcessParser::isPidExisting(std::string pid) { return 0; }