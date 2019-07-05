#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <string>

#include "constants.h"

using std::string;

class SystemParser {
 public:
  static long int UpTime();
  static int CpuCores();
};

int SystemParser::CpuCores() {
  string token;
  std::ifstream stream(Path::base + Path::cpuinfo);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "cores") {
        if (stream >> token >> token) return stoi(token);
      }
    }
  }
  return 0;

}

long int SystemParser::UpTime() {
  string token;
  std::ifstream stream(Path::base + Path::uptime);
  if (stream.is_open()) {
    if (stream >> token) {
      return stoi(token);
    }
  }
  return 0;
}

#endif