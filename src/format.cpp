#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  /* reference:
   * https://stackoverflow.com/questions/35608291/formatting-how-to-convert-1-to-01-2-to-02-3-to-03-and-so-on
   */

  std::stringstream ss;

  int hour = seconds / (60 * 60 * 1000);

  seconds -= hour * (60 * 60 * 1000);

  int minute = (seconds) / (60 * 1000);

  seconds -= minute * (60 * 1000);

  int second = (seconds) / 1000;

  ss << std::setw(2) << std::setfill('0') << std::to_string(hour);
  ss << ":" << std::setw(2) << std::to_string(minute);
  ss << ":" << std::setw(2) << std::to_string(second);

  return ss.str();
}