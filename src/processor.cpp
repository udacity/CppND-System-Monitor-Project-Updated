#include "processor.h"
#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"

float Processor::Utilization() {
  /*
  This implementation follows the proposal made by Vangelis Tasoulas
   at
  https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  */

  std::vector<unsigned long long int> line_read;

  line_read = LinuxParser::CpuUtilization();

  // I dont like this approach, but structured binding wouldnt work with vectors
  unsigned long long int user = line_read[0];
  unsigned long long int nice = line_read[1];
  unsigned long long int system = line_read[2];
  unsigned long long int idle = line_read[3];
  unsigned long long int iowait = line_read[4];
  unsigned long long int irq = line_read[5];
  unsigned long long int softirq = line_read[6];
  unsigned long long int steal = line_read[7];

  unsigned long long int current_idle = idle + iowait;
  unsigned long long int current_non_idle =
      user + nice + system + irq + softirq + steal;
  unsigned long long int current_total = current_idle + current_non_idle;

  unsigned long long int total_diff = current_total - last_total;
  unsigned long long int idle_diff = current_idle - last_idle;

  float cpu_use = (total_diff - idle_diff) / float(total_diff);

  // store current data to be used in the future
  last_idle = current_idle;
  last_total = current_total;

  return cpu_use;
}