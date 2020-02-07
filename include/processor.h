#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
private:
 long previous_cpu_usage_active_, previous_cpu_usage_idle_;
};

#endif