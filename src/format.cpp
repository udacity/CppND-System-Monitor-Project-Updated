#include <string>

#include "format.h"

using std::string;
using std::to_string;

std::string Format::Pad(string s, char c) {
  s.insert(s.begin(), 2 - s.size(), c);
  return s;
}

string Format::Time(long int time) {
  int hours = time / (60 * 60);
  int minutes = (time / 60) % 60;
  long seconds = time % 60;
  return std::string(Format::Pad(to_string(hours), '0') + ":" +
                     Format::Pad(to_string(minutes), '0') + ":" +
                     Format::Pad(to_string(seconds), '0'));
}