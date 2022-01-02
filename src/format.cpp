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

  int hours = seconds / (3600);

  seconds = seconds % (3600);

  int minutes = seconds / 60;

  seconds = seconds % 60;

  ss << std::setw(2) << std::setfill('0') << std::to_string(hours);
  ss << ":" << std::setw(2) << std::to_string(minutes);
  ss << ":" << std::setw(2) << std::to_string(seconds);

  return ss.str();
}