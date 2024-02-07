#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
  std::vector<std::string> currentUtilization = LinuxParser::CpuUtilization();
  if (currentUtilization.size() < 10) {
    return 0.0;
  }

  std::string user = currentUtilization[0];
  float nice = std::stoi(currentUtilization[1]);
  float system = std::stoi(currentUtilization[2]);
  float idle = std::stoi(currentUtilization[3]);
  float iowait = std::stoi(currentUtilization[4]);
  float irq = std::stoi(currentUtilization[5]);
  float softirq = std::stoi(currentUtilization[6]);
  float steal = std::stoi(currentUtilization[7]);

  float totalTime = nice + system + idle + iowait + irq + softirq + steal;
  float idleTime = idle + iowait;

  float deltaTimeTotal = totalTime - prevTotal_;
  float deltaTimeIdle = idleTime - prevIdle_;

  float cpuUtilization = (deltaTimeTotal != 0) ? (float)(deltaTimeTotal - deltaTimeIdle) / deltaTimeTotal : 0.0;

  prevTotal_ = totalTime;
  prevIdle_ = idleTime;

  return cpuUtilization;
}