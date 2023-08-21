#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{
    string time;
    string h = std::to_string(seconds / 3600);
    string m = std::to_string((seconds % 3600) / 60);
    string s = std::to_string(seconds % 60);
    std::string hh = std::string(2 - h.length(), '0') + h;
    std::string mm = std::string(2 - m.length(), '0') + m;
    std::string ss = std::string(2 - s.length(), '0') + s;
    time = hh + ":" + mm + ":" + ss;
    return time;
}