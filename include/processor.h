#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 // CPU calc aux
  int last_user = 0;
  int last_nice = 0;
  int last_system = 0;
  int last_idle = 0;
  int last_iowait = 0;
  int last_irq = 0;
  int last_softirq = 0;
  int last_steal = 0;
  int last_guest = 0;
  int last_gest_nice = 0;
};

#endif