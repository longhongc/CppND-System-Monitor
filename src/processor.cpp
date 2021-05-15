#include "processor.h"
#include "format.h"
#include "linux_parser.h"

float Processor::Utilization() {
  UpdateTimeInfo();
  double totald = nowtime_.cpu_time - prevtime_.cpu_time;
  double idled = nowtime_.idle_time - prevtime_.idle_time;
  double CPU_Percentage = 0.0; 
  if (totald > 0){
    CPU_Percentage = (totald - idled) / totald;
  }

  // temp is ahead of prev a cycle, now is ahead of temp a cycle 
  int temp_interval = nowtime_.uptime-temptime_.uptime; 
  if (temp_interval > CPU_UTIL_UPDATE_CYCLE){
      prevtime_ = temptime_; 
      temptime_ = nowtime_;
  }
  return (float)CPU_Percentage;
}

void Processor::UpdateTimeInfo() {
  nowtime_.uptime = LinuxParser::UpTime();

  auto cpu_data_string_vec = LinuxParser::CpuUtilization();
  if(cpu_data_string_vec.size() < 11){
      return; 
  }
  cpu_data_ = Format::CpuData(cpu_data_string_vec);
  double Idle = cpu_data_["idle"] + cpu_data_["iowait"];
  double NonIdle = cpu_data_["user"] + cpu_data_["nice"] + cpu_data_["system"] +
                   cpu_data_["irq"] + cpu_data_["softirq"] +
                   cpu_data_["steal"] - cpu_data_["guest"] -
                   cpu_data_["guest_nice"];

  nowtime_.idle_time = Idle;
  nowtime_.cpu_time = Idle + NonIdle;
}
