#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string;
using std::to_string;

void printAttrib(WINDOW* window, string label, string value) {
  wattron(window, COLOR_PAIR(2));
  wprintw(window, label.c_str());
  wattroff(window, COLOR_PAIR(2));
  wprintw(window, value.c_str());
}

void printAttrib(WINDOW* window, int row, int col, string label, string value) {
  mvwprintw(window, row, col, "");
  printAttrib(window, label, value);
}

void printProgressBar(WINDOW* window, int row, int col, string label, float pct) {
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, row, col, label.c_str());
  wattroff(window, COLOR_PAIR(2));
  mvwprintw(window, row, col+5, (Format::Percent(pct)+"% [").c_str());
  wattron(window, COLOR_PAIR(3));
  wprintw(window, Format::ProgressBar(pct).c_str());
  wattroff(window, COLOR_PAIR(3));
  wprintw(window, "]");
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{1};
  printAttrib(window, row, 2, "OS: ", system.OperatingSystem());
  printAttrib(window, " Kernel: ", system.Kernel());
  for (auto cpu : system.Cpu()) {
    printProgressBar(window, ++row, 2, cpu.CpuID(), cpu.Utilization());
  }
  printProgressBar(window, ++row, 2, "Mem", system.MemoryUtilization());
  printAttrib(window, ++row, 2, "Total/Running Processes: ", 
    to_string(system.TotalProcesses())+"/"+to_string(system.RunningProcesses()));
  printAttrib(window, " Up Time: ", Format::ElapsedTime(system.UpTime()));
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
        const long sys_uptime, WINDOW* window, int n, const char sort) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(sort == 'p' || sort == 'P' ? 3 : 2));
  mvwprintw(window, ++row, pid_column, "PID");
  wattron(window, COLOR_PAIR(sort == 'u' || sort == 'U' ? 3 : 2));
  mvwprintw(window, row, user_column, "USER");
  wattron(window, COLOR_PAIR(sort == 'c' || sort == 'C' ? 3 : 2));
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  wattron(window, COLOR_PAIR(sort == 'r' || sort == 'R' ? 3 : 2));
  mvwprintw(window, row, ram_column, "RAM[MB]");
  wattron(window, COLOR_PAIR(sort == 't' || sort == 'T' ? 3 : 2));
  mvwprintw(window, row, time_column, "TIME+");
  wattron(window, COLOR_PAIR(sort == 'm' || sort == 'M' ? 3 : 2));
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  for (int i = 0; i < num_processes; ++i) {
    mvwprintw(window, ++row, pid_column, Format::LeftPad(to_string(processes[i].Pid()), 6).c_str());
    mvwprintw(window, row, user_column, Format::RightPad(processes[i].User(), 8).c_str());
    mvwprintw(window, row, cpu_column,
              Format::Percent(processes[i].CpuUtilization()).c_str());
    mvwprintw(window, row, ram_column, Format::Megabyte(processes[i].Ram()).c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime(sys_uptime)).c_str());
    mvwprintw(window, row, command_column,
              Format::RightPad(processes[i].Command(), window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  nodelay(stdscr, true);
  start_color();  // enable color
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_CYAN, COLOR_BLACK);

  int x_max{getmaxx(stdscr)};
  int y_max{getmaxy(stdscr)};
  int cpuDispCount = std::min(static_cast<std::size_t>(5), system.Cpu().size());
  WINDOW* system_window = newwin(5 + cpuDispCount, x_max - 1, 0, 0);
  n = y_max - system_window->_maxy - 4;
  WINDOW* process_window = newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);
  char sort = 'C';
  char input = ' ';
  while (input != 'q') {
    system.UpdateStats(sort);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), system.UpTime(), process_window, n, sort);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    input = getch();
    if (input == 'c' || input == 'r' || input == 'm' || input == 'u' || input == 'p'
     || input == 't' || input == 'C' || input == 'R' || input == 'M' || input == 'U'
     || input == 'P' || input == 'T') sort = input;
  }
  endwin();
}
