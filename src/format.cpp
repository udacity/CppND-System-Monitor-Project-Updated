#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  return std::to_string(seconds / 3600) + ":" +
         std::to_string((seconds % 3600) / 60) + ":" +
         std::to_string(seconds % 60);
}