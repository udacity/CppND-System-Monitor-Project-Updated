#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long inputSeconds) {
	long hours   =0 ;
	long minutes =0 ;
	long seconds =0 ;
	string timeString;

	hours   =inputSeconds/HOUR;
	seconds =inputSeconds % HOUR;
	minutes =seconds/MIN;
	seconds %= MIN;

	timeString= std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

	return timeString;
}

