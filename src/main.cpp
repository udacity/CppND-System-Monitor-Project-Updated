#include "linux_system.h"
#include "ncurses_display.h"

int main() {
  LinuxSystem system;
  NCursesDisplay::Display(system);
}