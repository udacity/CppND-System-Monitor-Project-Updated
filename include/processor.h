#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  // CPU calc aux
  unsigned long long int last_total = 0;
  unsigned long long int last_idle = 0;
};

#endif