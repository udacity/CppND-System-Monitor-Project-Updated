#include "util.h"
#include <string>

using std::string;
using std::to_string;

string Util::ZeroPad(long number) {
  string padding;
  if (number < 10) padding = "0";
  return padding.append(to_string(number));
}

string Util::FormatTime(long int input_seconds) {
  long seconds = input_seconds % 60;
  long minutes = input_seconds / 60;
  long hours = minutes / 60;
  minutes = int(minutes % 60);

  return std::string(Util::ZeroPad(hours) + ":" + Util::ZeroPad(minutes) +
                     ":" + Util::ZeroPad(seconds));
}

// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
std::string Util::GetProgressBar(std::string percent) {
  std::string result{"0%"};
  int size{50};
  float boundaries{0};
  try {
    boundaries = (stof(percent) / 100) * size;
  } catch (...) {
    boundaries = 0;
  }

  for (auto i{0}; i < size; ++i) {
    if (i <= boundaries) {
      result += "|";
    } else {
      result += " ";
    }
  }

  return result + " " + percent.substr(0, 5) + " /100%";
}

// TODO: Move to ProcessParser
void Util::GetStream(std::string path, std::ifstream& stream) {
  stream.open(path, std::ifstream::in);
  if (!stream || !stream.is_open()) {
    stream.close();
    throw std::runtime_error("Non - existing PID");
  }
}