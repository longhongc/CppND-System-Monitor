#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <map>

#include "format.h"

using std::string;
using std::stoi; 
using std::vector; 

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    double days = double(seconds)/60/60/24; 
    double hours = (days - int(days))*24; 
    double minutes = (hours - int(hours))*60;
    seconds = (minutes - int(minutes))*60;
    std::ostringstream datetimestream; 
    datetimestream
      << std::setfill('0') << std::setw(2) << int(hours) << ":"
      << std::setfill('0') << std::setw(2) << int(minutes) << ":"  
      << std::setfill('0') << std::setw(2) << seconds; 
    return datetimestream.str(); 
}

std::map<string, double> Format::CpuData(vector<string> &cpu_data){
    std::map<string, double> cpu_data_map; 
    cpu_data_map["user"] = stoi(cpu_data[1])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["nice"] = stoi(cpu_data[2])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["system"] = stoi(cpu_data[3])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["idle"] = stoi(cpu_data[4])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["iowait"] = stoi(cpu_data[5])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["irq"] = stoi(cpu_data[6])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["softirq"] = stoi(cpu_data[7])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["steal"] = stoi(cpu_data[8])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["guest"] = stoi(cpu_data[9])/sysconf(_SC_CLK_TCK); 
    cpu_data_map["guest_nice"] = stoi(cpu_data[10])/sysconf(_SC_CLK_TCK); 
    return cpu_data_map; 
}
