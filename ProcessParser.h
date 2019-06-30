#ifndef PROCESS_PARSER_H
#define PROCESS_PARSER_H

#include "constants.h"
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

class ProcessParser {
private:
  std::ifstream stream;

public:
  static string getCmd(string pid) { return string(); }
  static vector<string> getPidList() { return vector<string> {}; }
  static std::string getVmSize(string pid) { return string(); }
  static std::string getCpuPercent(string pid) { return string(); }
  static long int getSysUpTime() { return 0; }
  static std::string getProcUpTime(string pid) { return 0; }
  static string getProcUser(string pid) { return 0; }
  static vector<string> getSysCpuPercent(string coreNumber = "") { return vector<string> {}; }
  static float getSysRamPercent() { return 0; }
  static string getSysKernelVersion() { return string(""); }
  static int getNumberOfCores() { return 0; }
  static int getTotalThreads() { return 0; }
  static int getTotalNumberOfProcesses() { return 0; }
  static int getNumberOfRunningProcesses() { return 0; }
  static string getOSName() { return string(""); }
  static std::string PrintCpuStats(std::vector<std::string> values1,
                                   std::vector<std::string> values2) { return string(""); }
  static bool isPidExisting(string pid) { return 0; }
};

// TODO: Define all of the above functions below:
#endif