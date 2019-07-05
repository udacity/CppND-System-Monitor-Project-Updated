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
  static std::string Cmd(std::string pid);
  static std::vector<std::string> getPidList();
  static std::string VmSize(std::string pid);
  static std::string Cpu(std::string pid);
  static long int getSysUpTime();
  static std::string UpTime(std::string pid);
  static std::string User(std::string pid);
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
  static bool isPidExisting(std::string pid); // TODO: Rename Exists()

 private:
  std::ifstream stream; // TODO: Remove
};

#endif