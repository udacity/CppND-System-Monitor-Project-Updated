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

std::string FormatPercent(float pct) {
  string display{to_string(pct * 100.0f).substr(0, 4)};
  if (pct < 0.1f || pct == 1.0f)
    display = " " + to_string(pct * 100.0f).substr(0, 3);
  return display+"%";
}

// renders progress bar with given width showing utilization
std::string NCursesDisplay::ProgressBar(float percent, int width) {
  std::string result;
  int bars = percent * width;

  for (int i{0}; i < width; ++i) {
    result += i <= bars ? '|' : ' ';
  }
  return result;
}

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
  mvwprintw(window, row, col+5, (FormatPercent(pct)+" [").c_str());
  wattron(window, COLOR_PAIR(3));
  wprintw(window, NCursesDisplay::ProgressBar(pct).c_str());
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
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  for (int i = 0; i < num_processes; ++i) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  int cpuDispCount = std::min(static_cast<std::size_t>(5), system.Cpu().size());
  WINDOW* system_window = newwin(5 + cpuDispCount, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    system.UpdateStats();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}
