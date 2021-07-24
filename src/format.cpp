#include <string>

#include "format.h"
#include "linux_parser.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string output = "";
    std::ostringstream outputstream(output);
    int hh, mm, ss;
    ss = seconds % 60;
    mm = int(seconds / 60) % 60;
    hh = int(seconds / 3600);
    outputstream << (hh < 10 ? "0" : string() ) << hh << (mm < 10 ? ":0" : ":" ) << mm << (ss < 10 ? ":0" : ":" ) << ss;

    return outputstream.str(); 
}