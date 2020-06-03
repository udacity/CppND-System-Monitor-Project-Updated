#ifndef FORMAT_H
#define FORMAT_H

//seconds in one hour
#define HOUR 3600
//seconds in one minute
#define MIN 60

#include <string>



namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif
