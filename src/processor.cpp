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

  std::vector<long int> line_read;

  line_read = LinuxParser::CpuUtilization();

  // I dont like this approach, but structured binding wouldnt work with vectors
  long int user = line_read[0];
  long int nice = line_read[1];
  long int system = line_read[2];
  long int idle = line_read[3];
  long int iowait = line_read[4];
  long int irq = line_read[5];
  long int softirq = line_read[6];
  long int steal = line_read[7];
  long int guest = line_read[8];
  long int gest_nice = line_read[9];

  long int current_idle = idle + iowait;
  long int current_non_idle = user + nice + system + irq + steal;
  long int current_total = current_idle + current_non_idle;

  long int last_idle_calc = last_idle + last_iowait;
  long int last_non_idle =
      last_user + last_nice + last_system + last_irq + last_steal;
  long int last_total = last_idle + last_non_idle;

  float cpu_use =
      float((current_total - last_total) - (current_idle - last_idle_calc)) /
      (current_total - last_total);

  // store current data to be used in the future
  last_nice = nice;
  last_system = system;
  last_idle = idle;
  last_iowait = iowait;
  last_irq = irq;
  last_softirq = softirq;
  last_steal = steal;
  last_guest = guest;
  last_gest_nice = gest_nice;

  return cpu_use;
}