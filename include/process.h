#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process (int p, std::string u, std::string com, std::string r, long int ut, float c) : pid(p), user(u), command(com), ram(r), upTime(ut), cpu(c) {}
    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp
    
  // TODO: Declare any necessary private members
 private:
    int pid;
    std::string user;
    std::string command;
    float cpu;
    std::string ram;
    long int upTime;

};

#endif