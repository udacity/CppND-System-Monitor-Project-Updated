#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process> const& processes, WINDOW* window,
                      int n = 10);
};  // namespace NCursesDisplay

#endif