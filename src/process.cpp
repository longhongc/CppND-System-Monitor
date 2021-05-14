#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

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
  // update temp before prev, to always remain at least 1 Cycle time between prev and now.
  if (time_interval > CPU_UTIL_UPDATE_CYCLE){
    temptime_ = nowtime_;
  }
  if (time_interval > CPU_UTIL_UPDATE_CYCLE * 2){
    prevtime_ = temptime_;
  }
  return (float)CPU_Percentage;
}

string Process::Command() const  { return LinuxParser::Command(pid_); }

string Process::Ram() const  { return LinuxParser::Ram(pid_); }

string Process::User() const  { return LinuxParser::User(pid_); }

long int Process::UpTime() const  { return LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK); }

bool Process::operator<(const Process &a) const {
  return pid_ < a.Pid();
  //return this->Ram() < a.Ram();
  //return this->cpu_percentage_ < a.cpu_percentage_; 
}

bool Process::operator>(const Process &a) const {
  return pid_ > a.Pid();
  //return this->Ram() > a.Ram();
  //return this->cpu_percentage_ > a.cpu_percentage_; 
}

bool Process::operator==(const Process &a) const {
  return pid_ == a.Pid();
  //return this->Ram() == a.Ram();
  //return this->cpu_percentage_ > a.cpu_percentage_; 
}

void Process::UpdateTimeInfo() {
  if (prevtime_.uptime == 0) {
    prevtime_.uptime = this->UpTime();
  }
  nowtime_.uptime = LinuxParser::UpTime();
  auto process_data_string_vec = LinuxParser::ProcessUtilization(pid_);
  process_data_ = Format::ProcessData(process_data_string_vec);
  double used_time = process_data_["utime"] + 
                     process_data_["stime"];  
                     //process_data_["cutime"] +
                     //process_data_["cstime"];
  nowtime_.used_time = used_time;
}
