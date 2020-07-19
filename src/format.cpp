#include <string>

#include "format.h"

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours = seconds/3600;
    seconds -= hours*3600;
    int minutes = seconds/60;
    seconds -= minutes*60;
    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds); 
}