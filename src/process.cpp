#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "format.h"
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid){
    prevtime_.uptime = LinuxParser::StartTime(pid_);
    temptime_.uptime = prevtime_.uptime; 
}

int Process::Pid() const  { return pid_; }

float Process::CpuUtilization() {
  UpdateTimeInfo();
  int time_interval = nowtime_.uptime - prevtime_.uptime;
  double used_time = nowtime_.used_time - prevtime_.used_time;
  double CPU_Percentage = 0.0;  
  if(time_interval > 0){
    CPU_Percentage = used_time / time_interval;
  }
  cpu_percentage_ = CPU_Percentage; 
  // temp is ahead of prev a cycle, now is ahead of temp a cycle 
  int temp_interval = nowtime_.uptime-temptime_.uptime; 
  // only activate this when cpu usage is high
  if (temp_interval > CPU_UTIL_UPDATE_CYCLE && cpu_percentage_ > 0.1){
      prevtime_ = temptime_; 
      temptime_ = nowtime_;
  }
  return static_cast<float>(CPU_Percentage);
}

string Process::Command() const  { return LinuxParser::Command(pid_); }

string Process::Ram() const  { return LinuxParser::Ram(pid_); }

string Process::User() const  { return LinuxParser::User(pid_); }

long int Process::UpTime() const  {
    auto system_uptime = LinuxParser::UpTime(); 
    auto process_starttime = LinuxParser::StartTime(pid_);
    return system_uptime - process_starttime; 
}

bool Process::operator<(const Process &a) const {
  //return pid_ < a.Pid();
  //return this->Ram() < a.Ram();
  return this->cpu_percentage_ < a.cpu_percentage_; 
}

bool Process::operator>(const Process &a) const {
  //return pid_ > a.Pid();
  //return this->Ram() > a.Ram();
  return this->cpu_percentage_ > a.cpu_percentage_; 
}

bool Process::operator==(const Process &a) const {
  return pid_ == a.Pid();
}

void Process::UpdateTimeInfo() {
  nowtime_.uptime = LinuxParser::UpTime();
  auto process_data_string_vec = LinuxParser::ProcessUtilization(pid_);
  if (process_data_string_vec.size() < 22){
      return; 
  }
  process_data_ = Format::ProcessData(process_data_string_vec);
  double used_time = process_data_["utime"] + 
                     process_data_["stime"] + 
                     process_data_["cutime"] +
                     process_data_["cstime"];
  nowtime_.used_time = used_time;
}
