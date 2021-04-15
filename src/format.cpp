#include <string>

#include "format.h"

using std::string;

string zeroPadTime(int t) {
  return (t>9 ? "" : "0")+std::to_string(t);
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
