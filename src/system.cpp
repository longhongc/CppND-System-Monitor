#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

float System::CpuUtilization() {
    return cpu_.Utilization(); 
}

vector<Process>& System::Processes() {
    processes_.clear(); 
    auto pids = LinuxParser::Pids(); 
    std::map<int, Process > new_process_map = {}; 
    for(const auto &pid: pids){
        if(process_map_.find(pid) != process_map_.end()){
            new_process_map[pid] = process_map_[pid]; 
        }else{
            Process process{pid}; 
            new_process_map[pid] = process; 
        }
    }
    for(const auto process: new_process_map){
        processes_.push_back(process.second); 
    }
    process_map_ = new_process_map; 
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>()); 
    return processes_; 
}

std::string System::Kernel() {
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() {
    return LinuxParser::UpTime(); 
}

