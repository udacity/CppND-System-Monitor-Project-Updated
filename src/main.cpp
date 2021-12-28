#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "ncurses_display.h"
#include "processor.h"
#include "system.h"

int main() {
  System system;

  NCursesDisplay::Display(system);
}