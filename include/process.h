#ifndef PROCESS_H
#define PROCESS_H

#include <map>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() {}
  Process(int pid) : pid_(pid) {}
  int Pid() const ;
  std::string User() const ;
  std::string Command() const ;
  float CpuUtilization();
  std::string Ram() const ;
  long int UpTime() const ;
  bool operator<(const Process &a) const ;
  bool operator==(const Process &a) const ;
  bool operator>(const Process &a) const ;
  double cpu_percentage_{0.0}; 
  ~Process(){}
  

 private:
  int pid_{0};

  std::map<std::string, double> process_data_;
  void UpdateTimeInfo();
  static const int CPU_UTIL_UPDATE_CYCLE = 10;
  struct TimeInfo {
    int uptime{0};
    double used_time{0};
  };
  TimeInfo prevtime_;
  TimeInfo temptime_;
  TimeInfo nowtime_;
};

#endif
