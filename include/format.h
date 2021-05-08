#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
  std::string ElapsedTime(long times);  // See src/format.cpp

  std::string Percent(const float pct);

  std::string Megabyte(const int ram_kb);

  std::string ProgressBar(const float percent, const int width = 32);

  std::string LeftPad(std::string num, const int width);

  std::string RightPad(std::string text, const int width);

};     // namespace Format
#endif
