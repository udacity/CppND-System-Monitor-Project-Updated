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
  static std::string Cmdline(std::string pid);
  static std::vector<std::string> Pids();
  static std::string VmSize(std::string pid);
  static std::string Cpu(std::string pid);
  static long int getSysUpTime();
  static std::string UpTime(std::string pid);
  static std::string Uid(std::string pid);
  static std::string User(std::string pid);
  static std::vector<std::string> getSysCpuPercent(std::string coreNumber);
  static bool isPidExisting(std::string pid);  // TODO: Rename Exists()
  static std::string Threads(std::string pid);

 private:
  std::ifstream stream;  // TODO: Remove
};

#endif