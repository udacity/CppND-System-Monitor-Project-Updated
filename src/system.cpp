#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include <algorithm>

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    
    vector<int> nPids, oPids = {}; //new pids, old pids
    auto pids = LinuxParser::Pids();
    bool match = false;

    //Check for new processes
    for(int pid : pids) {
        for(unsigned int i = 0; i < pids_.size(); i++) {
            if(pid == pids[i]) {
                oPids.emplace_back(pid);
                match = true;
                break;
            }
        }
        if(!match) { 
            nPids.emplace_back(pid);
        }
        match = false;
    }

    //Delete missing processes
    for(unsigned int j = 0; j < processes_.size(); j++) {
        for(auto oPid: oPids) {
            if(processes_[j] == oPid) {
                match = true;
                break;
            }
        }
        if(!match) {
            processes_.erase(processes_.begin() + j);
        }
        match = false;
    }

    //Add new processes
    for(auto nPid: nPids) {
        processes_.emplace_back(Process(nPid));
    }

    pids_ = pids;

    for(unsigned int p = 0; p < processes_.size(); p++) {
        processes_[p].UpdateCPU();
    }

    std::sort(processes_.begin(), processes_.end(), [](Process &a, Process &b){return a<b;});

    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
