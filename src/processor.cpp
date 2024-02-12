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
  float iowait = 0.0;  // std::stoi(currentUtilization[4]);
  float irq = std::stoi(currentUtilization[5]);
  float softirq = std::stoi(currentUtilization[6]);
  float steal = std::stoi(currentUtilization[7]);

  // try {
  //   int num = std::stoi(iowait);
  // } catch (std::out_of_range& e) {
  //   std::cerr << "Number is out of range: " << e.what() << '\n';
  // }

  float totalTime = nice + system + idle + iowait + irq + softirq + steal;
  float idleTime = idle + iowait;

  float deltaTimeTotal = totalTime - prevTotal_;
  float deltaTimeIdle = idleTime - prevIdle_;

  float cpuUtilization;
  if (deltaTimeTotal != 0) {
    cpuUtilization = (float)(deltaTimeTotal - deltaTimeIdle) / deltaTimeTotal;
  } else {
    cpuUtilization = 0.0;
  }
  prevTotal_ = totalTime;
  prevIdle_ = idleTime;

  return cpuUtilization;
}