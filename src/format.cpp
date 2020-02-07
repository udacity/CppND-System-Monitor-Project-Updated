#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    
    char elapsedTime [8];
    int hh = seconds / 3600;
    int mm = seconds % 3600 / 60;
    int ss = seconds % 60;
    sprintf(elapsedTime, "%d:%d:%d", hh, mm, ss);
    return elapsedTime;
}