#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor() : prevTotal_(0), prevIdle_(0){};
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevTotal_;
  float prevIdle_;
};

#endif
