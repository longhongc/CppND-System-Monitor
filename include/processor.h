#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <map>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

 private:
  std::map<std::string, double> cpu_data_; 

  void UpdateTimeInfo(); 
  const int CPU_UTIL_UPDATE_CYCLE = 10; 
  struct TimeInfo{
    int uptime{0}; 
    double cpu_time{0}; 
    double idle_time{0}; 
  };
  TimeInfo prevtime_;
  TimeInfo temptime_; 
  TimeInfo nowtime_; 
};

#endif
