#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "constants.h"

class SystemParser {
 public:
  static long int UpTime();
  static std::vector<std::string> AggregateCpuUtilization();
  static std::vector<std::vector<std::string>> IndividualCpuUtilizations();
  static float CpuUtilization(std::vector<std::string> time1,
                              std::vector<std::string> time2);
  static float MemoryUtilization();
  static std::vector<std::string> Lines(std::string filepath);

 private:
  static long Active(std::vector<std::string> cpu);
  static long Idle(std::vector<std::string> cpu);
  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
  static CPUStates cpu_states_;
};

#endif