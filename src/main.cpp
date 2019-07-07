#include <ncurses.h>
#include <time.h>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "process_parser.h"
#include "system.h"
#include "util.h"

void DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "Aggregate CPU: ");
  wattron(window, COLOR_PAIR(1));
  wprintw(window,
          Util::GetProgressBar(system.AggregateCpuUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Individual CPUs:");
  wattron(window, COLOR_PAIR(1));
  std::vector<std::string> val = system.IndividualCpuUtilizations();
  for (std::size_t i = 0; i < val.size(); i++) {
    mvwprintw(window, ++row, 2, "");
    wprintw(window, Util::GetProgressBar(val[i]).c_str());
  }
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  wprintw(window, Util::GetProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + system.TotalProcesses()).c_str());
  mvwprintw(window, ++row, 2,
            ("Running Processes: " + system.RunningProcesses()).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Util::FormatTime(system.UpTime())).c_str());
  wrefresh(window);
}

void DisplayProcesses(const std::vector<Process>& processes, WINDOW* window) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{44};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, 2, "PID");
  mvwprintw(window, row, 9, "USER");
  mvwprintw(window, row, 16, "CPU[%%]");
  mvwprintw(window, row, 26, "RAM[MB]");
  mvwprintw(window, row, 35, "TIME+");
  mvwprintw(window, row, 44, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  for (int i = 0; i < 10; i++) {
    mvwprintw(window, ++row, pid_column, processes[i].Pid().c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    mvwprintw(window, row, cpu_column,
              processes[i].CpuUtilization().substr(0, 5).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column, processes[i].UpTime().c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 44).c_str());
  }
}

void Display(System& system) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(14, x_max - 1, 0, 0);
  WINDOW* process_window = newwin(13, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    sleep(1);
  }
  endwin();
}

int main() {
  System system;
  Display(system);
}