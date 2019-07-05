#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <string>

// Classic helper function
class Util {
 public:
  static std::string FormatTime(long int input_seconds);
  static std::string GetProgressBar(std::string percent);
  static void GetStream(std::string path, std::ifstream& stream);

 private:
  static std::string ZeroPad(long int number);
};

#endif