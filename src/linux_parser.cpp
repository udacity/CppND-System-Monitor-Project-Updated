#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
					std::replace(value.begin(), value.end(), '_', ' ');
					return value;
				}
			}
		}
	}
	return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
	string os, kernel;
	string line;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> os >> kernel;
	}
	return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
	vector<int> pids;
	DIR* directory = opendir(kProcDirectory.c_str());
	struct dirent* file;
	while ((file = readdir(directory)) != nullptr) {
		// Is this a directory?
		if (file->d_type == DT_DIR) {
			// Is every character of the name a digit?
			string filename(file->d_name);
			if (std::all_of(filename.begin(), filename.end(), isdigit)) {
				int pid = stoi(filename);
				pids.push_back(pid);
			}
		}
	}
	closedir(directory);
	return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

	string line;
	string key;
	string kb;
	float MemTotal;
	float MemFree;

	std::ifstream stream(kProcDirectory + kMeminfoFilename);
	if (stream.is_open()) {
		// Get Total Memory
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> key >> MemTotal >> kb;

		// Get Free Memory
		std::getline(stream, line);
		linestream.str(line);
		linestream.clear();
		linestream >> key >> MemFree >> kb;
	}




	return((MemTotal - MemFree)/MemTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
	string line;
	long sysIdletime;
	long sysUptime;

	std::ifstream stream(kProcDirectory + kUptimeFilename);
	if (stream.is_open()) {
		// Get Total Memory
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> sysUptime >>sysIdletime ;
	}


	return (sysUptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
	return (LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies());
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
	/*user,nice,system,irq,softirq,steal,guest are all active*/

	long activeJiffies = 0;
	long userjif = 		std::stol(LinuxParser::CpuUtilization()[USER]);
	long nicejif =		 std::stol(LinuxParser::CpuUtilization()[NICE]);
	//long systemjif = 		std::stol(LinuxParser::CpuUtilization()[SYSTEM]); suppress warning about unused variable
	long irqjif = 		std::stol(LinuxParser::CpuUtilization()[IRQ]);
	long softirqjif = 	std::stol(LinuxParser::CpuUtilization()[SOFTIRQ]);
	long stealjif = 		std::stol(LinuxParser::CpuUtilization()[STEAL]);
	long guestjf = 		std::stol(LinuxParser::CpuUtilization()[GUEST]);
	long guestnicejf = 	std::stol(LinuxParser::CpuUtilization()[GUESTNICE]);

	activeJiffies = userjif+nicejif+irqjif+softirqjif+stealjif+guestjf+guestnicejf;
	return activeJiffies;


}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
	/*idle and iowait are both idle*/
	long idleJiffies = 0;


	long idle = 		std::stol(LinuxParser::CpuUtilization()[IDLE]);
	long ioWait = 	std::stol(LinuxParser::CpuUtilization()[IOWAIT]);

	idleJiffies = idle+ioWait;
	return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
	string line,name;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	long user = 0;
	long nice = 0;
	long system = 0;
	long idle = 0;
	long iowait = 0;
	long irq = 0;
	long softirq = 0;
	long steal = 0;
	long guest = 0;
	long guest_nice = 0;
	vector<string> CpuUtil;
	std::vector<long> cpu;


	if(filestream.is_open())
	{
	      while (std::getline(filestream, line))
	      {
	        std::istringstream linestream(line);
			linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

	      }
	}

	CpuUtil.push_back(to_string(user));
	CpuUtil.push_back(to_string(nice));
	CpuUtil.push_back(to_string(system));
	CpuUtil.push_back(to_string(idle));
	CpuUtil.push_back(to_string(iowait));
	CpuUtil.push_back(to_string(irq));
	CpuUtil.push_back(to_string(softirq));
	CpuUtil.push_back(to_string(steal));
	CpuUtil.push_back(to_string(guest));
	CpuUtil.push_back(to_string(guest_nice));



	return (CpuUtil);
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	string line;
	string key;
	int value;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "processes") {

					return value;
				}
			}
		}
	}
	filestream.close();
	return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	string line;
	string key;
	int value;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "procs_running") {

					return value;
				}
			}
		}
	}
	filestream.close();
	return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
	///proc/[pid]/cmdline
	string line;
	std::ifstream filestream(kProcDirectory+std::to_string(pid) + kCmdlineFilename);
	if (filestream.is_open()) {
		std::getline(filestream, line);
	}

	return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
	string line;
	string name;
	string size;
	string units;
	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
	if (filestream.is_open())
	{
		while (std::getline(filestream, line))
		{
			std::istringstream linestream(line);
			while (linestream >> name >> size >> units)
			{
				if (name == "VmSize:")
				{

					return size;
				}
			}
		}
	}
	return size;

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
	string line;
	string value;
	string key;
	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
	if (filestream.is_open())
	{
		while (std::getline(filestream, line))
		{
			std::istringstream linestream(line);
			while (linestream >> key >> value)
			{
				if (key == "Uid:")
				{

					return value;
				}
			}
		}
	}
	return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
	string line;
	std::ifstream filestream(kPasswordPath);
	string find_pid_name = "x" + LinuxParser::Uid(pid);
	if (filestream.is_open())
	{
		std::getline(filestream, line);
		std::istringstream linestream(line);
		if(line.find(find_pid_name) != std::string::npos)
		{
			return line.substr(0,line.find(":"));
		}

	}
	return line;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
	long  starttime;
	string line;
	std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
	if (filestream.is_open())
	{
		std::istringstream linestream(line);
		for(int idx =0; idx<START_TIME_TOKEN;idx++)
		{
			linestream >> starttime;
		}

	}
	return long(starttime/sysconf(_SC_CLK_TCK));
}
