#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string Time(long time);
std::string Pad(std::string s, char c);
};  // namespace Format

#endif