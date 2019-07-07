#include "ncurses.h"
#include "system.h"

int main() {
  System system;
  NCurses::Display(system, 10);
}