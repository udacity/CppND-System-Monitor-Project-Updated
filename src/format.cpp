#include "format.h"

#include <sstream>
#include <string>

using std::string;
using std::stringstream;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long times) {
  stringstream res;
  // add "HH:" to res
  res << times / 3600 << ":";
  // add "MM:" to res
  res << (times % 3600) / 60 << ":";
  // add "SS" to res
  res << (times % 3600) % 60;
  return res.str();
}