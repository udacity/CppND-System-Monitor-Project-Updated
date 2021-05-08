#include <string>

#include "format.h"

using std::string;
using std::to_string;

string zeroPadTime(int t) {
  return (t>9 ? "" : "0")+to_string(t);
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS (ignoring days)
string Format::ElapsedTime(long seconds) {
  string str {":"+zeroPadTime(seconds%60)};
  seconds/=60;
  str=":"+zeroPadTime(seconds%60)+str;
  seconds/=60;
  str=zeroPadTime(seconds%24)+str;
  return str;
}

string Format::Percent(const float pct) {
  string display{to_string(pct * 100.0f).substr(0, 4)};
  if (pct < 0.1f || pct == 1.0f)
    display = " " + to_string(pct * 100.0f).substr(0, 3);
  return display;
}

// left padded with space to 8 chars
string Format::Megabyte(const int ram_kb) {
  string val {to_string(ram_kb/1024).substr(0, 8)};
  if (val.size()<8) val = string(8-val.size(), ' ') + val;
  return val;
}

// renders progress bar with given width showing utilization percentage
string Format::ProgressBar(const float percent, const int width) {
  std::string result;
  int bars = percent * width;

  for (int i{0}; i < width; ++i) {
    result += i <= bars ? '|' : ' ';
  }
  return result;
}

// truncates string to fit specified width, left pads with spaces if shorter
string Format::LeftPad(string num, const int width) {
  return ( (int)num.size() < width ? string(width-num.size(), ' ')+num : num.substr(0, width) );
}

// truncates string to fit specified width, left pads with spaces if shorter
string Format::RightPad(string text, const int width) {
  return ( (int)text.size() < width ? text+string(width-text.size(), ' ') : text.substr(0, width) );
}
