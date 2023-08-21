#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
    float prev_total_jiffies_;
    float prev_active_jiffies_;
};

#endif