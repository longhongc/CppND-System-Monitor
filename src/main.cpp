#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  auto processes = system.Processes(); 
  NCursesDisplay::Display(system);
}
