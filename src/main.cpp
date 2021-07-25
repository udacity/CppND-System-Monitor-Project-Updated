#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"

int main() {
  System system;
  NCursesDisplay::Display(system);

  // std::cout << LinuxParser::kProcDirectory + std::to_string(1250) + LinuxParser::kStatusFilename << std::endl;
  // for(Process proc: system.Processes()) 
  //   std::cout << proc.Pid() << ": " << proc.User() << std::endl;

  // for(Process proc: system.Processes()) 
  //   std::cout << proc.Pid() << ": " << proc.User() << "  " << proc.UpTime() <<std::endl;

}