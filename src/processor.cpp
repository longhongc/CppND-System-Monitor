#include "processor.h"
#include "linux_parser.h"
#include "format.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    UpdateTimeInfo(); 
    int time_interval = nowtime_.uptime - prevtime_.uptime; 
    double totald = nowtime_.cpu_time - prevtime_.cpu_time; 
    double idled = nowtime_.idle_time - prevtime_.idle_time; 
    auto CPU_Percentage = (totald - idled)/totald; 
    if (time_interval > CPU_UTIL_UPDATE_CYCLE){
        prevtime_ = nowtime_; 
    }
    return (float)CPU_Percentage; 
}

void Processor::UpdateTimeInfo(){
    nowtime_.uptime = LinuxParser::UpTime(); 

    auto cpu_data_string_vec = LinuxParser::CpuUtilization(); 
    cpu_data_ = Format::CpuData(cpu_data_string_vec); 
    double Idle = cpu_data_["idle"] + cpu_data_["iowait"]; 
    double NonIdle = cpu_data_["user"] + 
                     cpu_data_["nice"] + 
                     cpu_data_["system"] + 
                     cpu_data_["irq"] + 
                     cpu_data_["softirq"] + 
                     cpu_data_["steal"] - 
                     cpu_data_["guest"] - 
                     cpu_data_["guest_nice"];  

    nowtime_.idle_time = Idle; 
    nowtime_.cpu_time = Idle + NonIdle; 
}

