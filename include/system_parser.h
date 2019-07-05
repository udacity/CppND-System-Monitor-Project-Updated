#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <string>

#include "constants.h"

using std::string;

class SystemParser {
 public:
  static long int UpTime();
};

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