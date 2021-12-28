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

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    
    //vector<Process> procs;
    std::sort(pids.begin(), pids.end());
    for ( uint i =0 ; i < pids.size(); i++){
        Process proc(pids[i]);
        processes_.push_back(proc);
    }
    
    return processes_; }

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses();; }

long int System::UpTime() { return LinuxParser::UpTime(); }