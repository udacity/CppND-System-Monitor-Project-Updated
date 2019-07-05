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

#include "process_container.h"
#include "process_parser.h"
#include "system.h"
#include "util.h"

using std::size_t;

char *getCString(std::string str) {
  char *cstr = new char[str.length() + 1];
  std::strcpy(cstr, str.c_str());
  return cstr;
}

void writeSysInfoToConsole(System sys, WINDOW *sys_win) {
  sys.Refresh();

  mvwprintw(sys_win, 2, 2, getCString(("OS: " + sys.OperatingSystem())));
  mvwprintw(sys_win, 3, 2, getCString(("Kernel: " + sys.Kernel())));
  mvwprintw(sys_win, 4, 2, getCString("CPU: "));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, getCString(Util::GetProgressBar(sys.Cpu())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 5, 2, getCString(("Other cores:")));
  wattron(sys_win, COLOR_PAIR(1));
  std::vector<std::string> val = sys.getCoresStats();
  for (size_t i = 0; i < val.size(); i++) {
    mvwprintw(sys_win, (6 + i), 2, getCString(val[i]));
  }
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 10, 2, getCString(("Memory: ")));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, getCString(Util::GetProgressBar(sys.MemoryUtilization())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 11, 2,
            getCString(("Total Processes: " + sys.TotalProcesses())));
  mvwprintw(sys_win, 12, 2,
            getCString(("Running Processes: " + sys.RunningProcesses())));
  mvwprintw(sys_win, 13, 2,
            getCString(("Up Time: " + Util::FormatTime(sys.UpTime()))));
  wrefresh(sys_win);
}

void getProcessListToConsole(ProcessContainer procs, WINDOW *win) {
  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 1, 2, "PID:");
  mvwprintw(win, 1, 9, "User:");
  mvwprintw(win, 1, 16, "CPU[%%]:");
  mvwprintw(win, 1, 26, "RAM[MB]:");
  mvwprintw(win, 1, 35, "Uptime:");
  mvwprintw(win, 1, 44, "CMD:");
  wattroff(win, COLOR_PAIR(2));
  auto processes = procs.Processes();
  for (int i = 0; i < 10; i++) {
    processes[i].Refresh();
    mvwprintw(win, 2 + i, 2, processes[i].Pid().c_str());
    mvwprintw(win, 2 + i, 9, processes[i].User().c_str());
    mvwprintw(win, 2 + i, 16, processes[i].Cpu().c_str());
    mvwprintw(win, 2 + i, 26, processes[i].Ram().c_str());
    mvwprintw(win, 2 + i, 35, processes[i].UpTime().c_str());
    mvwprintw(win, 2 + i, 44, processes[i].Cmd().c_str());
  }
}

void PrintMain(System system, ProcessContainer processes) {
  initscr();                   /* Start curses mode 		  */
  noecho();                    // not printing input values
  cbreak();                    // Terminating on classic ctrl + c
  start_color();               // Enabling color change of text
  int xMax = getmaxx(stdscr);  // getting size of window measured in lines and
                               // columns(column one char length)
  WINDOW *sys_win = newwin(15, xMax - 1, 0, 0);
  WINDOW *proc_win = newwin(13, xMax - 1, 16, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  while (1) {
    box(sys_win, 0, 0);
    box(proc_win, 0, 0);
    processes.Refresh();
    writeSysInfoToConsole(system, sys_win);
    getProcessListToConsole(processes, proc_win);
    wrefresh(sys_win);
    wrefresh(proc_win);
    refresh();
    sleep(1);
  }
  endwin();
}

int main() {
  // Object which contains list of current processes, Container for Process
  // Class
  ProcessContainer processes;
  // Object which containts relevant methods and attributes regarding system
  // details
  System sys;
  // std::string s = writeToConsole(sys);
  PrintMain(sys, processes);
}