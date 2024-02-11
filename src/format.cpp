#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string timeFormat;
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int sec = seconds % 60;
  timeFormat = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(sec);
  
  return timeFormat;
}