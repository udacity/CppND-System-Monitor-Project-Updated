#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"


using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(): os_(LinuxParser::OperatingSystem()), kernel_(LinuxParser::Kernel())
{

        // Initialize processor
        Processor processor;
       // cpu_ = processor;

        // Initialize processes
        vector<int> pids = LinuxParser::Pids();
        for (int pid : pids) {
            processes_.push_back(Process(pid));
        }

    };


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {

	return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {    return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
