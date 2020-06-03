#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

	float totalTime = LinuxParser::Jiffies();
	float activeTime = LinuxParser::ActiveJiffies();
	cpu_ = activeTime/totalTime;
	return cpu_;
}
