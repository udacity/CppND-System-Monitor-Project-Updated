#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

	float totalTime = LinuxParser::Jiffies();
	float idleTime = LinuxParser::IdleJiffies();
	cpu_ = ((totalTime - idleTime)/totalTime);
	return cpu_;
}
