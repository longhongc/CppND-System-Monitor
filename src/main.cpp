#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <chrono>
#include <thread>

#define debug(x) std::cout<< "Var " << (#x) << ": " << x << std::endl  

int main() {
  //std::chrono::milliseconds timespan(500); 
  System system;
  auto processes = system.Processes(); 
  //while(true){
  //    for(auto &process: processes){
  //        debug(process->Pid()); 
  //        process->CpuUtilization(); 
  //        debug(process->cpu_percentage_); 
  //    }
  //    std::cout << "------------" << std::endl; 
  //    std::this_thread::sleep_for(timespan); 
  //    processes = system.Processes(); 
  //}
  NCursesDisplay::Display(system);
}
