#ifndef PROCESS_PARSER_H
#define PROCESS_PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include "constants.h"

class ProcessParser {
 public:
  static std::string Cmdline(std::string pid);
  static std::vector<std::string> Pids();
  static std::string VmSize(std::string pid);
  static long Jiffies(std::string pid);
  static std::string UpTime(std::string pid);
  static std::string Uid(std::string pid);
  static std::string User(std::string pid);

 private:
  std::ifstream stream;  // TODO: Remove
};

#endif