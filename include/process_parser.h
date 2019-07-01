#ifndef PROCESS_PARSER_H
#define PROCESS_PARSER_H

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include "constants.h"

class ProcessParser {
 public:
  static std::string getCmd(std::string pid);
  static std::vector<std::string> getPidList();
  static std::string VmSize(std::string pid);
  static std::string getCpuPercent(std::string pid);
  static long int getSysUpTime();
  static std::string getProcUpTime(std::string pid);
  static std::string getProcUser(std::string pid);
  static std::vector<std::string> getSysCpuPercent(std::string coreNumber);
  static float getSysRamPercent();
  static std::string getSysKernelVersion();
  static int getNumberOfCores();
  static int getTotalThreads();
  static int getTotalNumberOfProcesses();
  static int getNumberOfRunningProcesses();
  static std::string getOSName();
  static std::string PrintCpuStats(std::vector<std::string> values1,
                                   std::vector<std::string> values2);
  static bool isPidExisting(std::string pid);

 private:
  std::ifstream stream;
};

#endif