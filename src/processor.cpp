#include "processor.h"
#include "format.h"
#include "linux_parser.h"

float Processor::Utilization() {
  UpdateTimeInfo();
  int time_interval = nowtime_.uptime - prevtime_.uptime;
  double totald = nowtime_.cpu_time - prevtime_.cpu_time;
  double idled = nowtime_.idle_time - prevtime_.idle_time;
  double CPU_Percentage = 0.0; 
  if (totald > 0){
    CPU_Percentage = (totald - idled) / totald;
  }

  // update temp before prev, to always remain at least 1 Cycle time between
  // prev and now.
  if (time_interval > CPU_UTIL_UPDATE_CYCLE) {
    temptime_ = nowtime_;
  }
  if (time_interval > CPU_UTIL_UPDATE_CYCLE * 2) {
    prevtime_ = temptime_;
  }
  return (float)CPU_Percentage;
}

void Processor::UpdateTimeInfo() {
  nowtime_.uptime = LinuxParser::UpTime();

  auto cpu_data_string_vec = LinuxParser::CpuUtilization();
  cpu_data_ = Format::CpuData(cpu_data_string_vec);
  double Idle = cpu_data_["idle"] + cpu_data_["iowait"];
  double NonIdle = cpu_data_["user"] + cpu_data_["nice"] + cpu_data_["system"] +
                   cpu_data_["irq"] + cpu_data_["softirq"] +
                   cpu_data_["steal"] - cpu_data_["guest"] -
                   cpu_data_["guest_nice"];

  nowtime_.idle_time = Idle;
  nowtime_.cpu_time = Idle + NonIdle;
}
