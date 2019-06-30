#include "process_parser.h"
#include <string>
#include <vector>
#include "util.h"

std::string ProcessParser::getCmd(std::string pid) { return std::string(); }
std::vector<std::string> ProcessParser::getPidList() {
  return std::vector<std::string>{};
}

// TODO: Refactor
std::string ProcessParser::getVmSize(std::string pid) {
  std::string line;
  std::string name{"VmData"};
  std::string value;
  float result;

  std::ifstream stream;
  Util::GetStream(Path::base_path + pid + Path::status_path, stream);
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream buf(line);
      std::istream_iterator<std::string> beg(buf), end;
      std::vector<std::string> values(beg, end);
      result = (stof(values[1]) / float(1024));
      break;
    }
  }

  return std::to_string(result);
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