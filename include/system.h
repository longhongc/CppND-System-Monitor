#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"



class System {
 public:
  float CpuUtilization();             
  std::vector<Process>& Processes();  //ToDo
  float MemoryUtilization();        
  long UpTime();                   
  int TotalProcesses();           
  int RunningProcesses();        
  std::string Kernel();         
  std::string OperatingSystem();

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

};

#endif
