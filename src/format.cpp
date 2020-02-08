#include <string>
#include <sstream>
#include "format.h"

using std::string;
using std::stringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) { 
    stringstream res;
    // add "HH:" to res
    res << times/3600 << ":";
    // add "MM:" to res
    res << (times % 3600)/60 << ":";
    // add "SS" to res
    res << (times % 3600)%60;
    
    return res.str();
}